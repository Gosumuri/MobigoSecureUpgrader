////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "UtilAccount.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

#define GROUP_ADMINISTRATORS    _T("Administrators")
#define GROUP_GUESTS            _T("Guests")

////////////////////////////////////////////////////////////////////////////////
// static member
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CUtilAccount::CUtilAccount(void)
{
    
}

CUtilAccount::~CUtilAccount(void)
{
}

////////////////////////////////////////////////////////////////////////////////
// Elevate( normal user -> administrator )
// http://miraggool.tistory.com/26
////////////////////////////////////////////////////////////////////////////////

BOOL CUtilAccount::ElevateAuth(TOKEN_ELEVATION_TYPE *pElevationType, BOOL *pIsAdmin)
{
    HANDLE hToken = NULL;
    BOOL bResult = FALSE;
    DWORD dwSize = 0;
 
    // 현재 프로세스의 토큰을 얻는다.
    if ( !OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken) )
        return FALSE;
 
    // 권한상승 형태에 대한 정보를 얻는다.
    if ( GetTokenInformation(hToken, TokenElevationType, pElevationType, sizeof(TOKEN_ELEVATION_TYPE), &dwSize) )
    {
        BYTE adminSID[SECURITY_MAX_SID_SIZE];
        dwSize = sizeof(adminSID);
         
        // 관리자 그룹의 SID 값을 생성한다.
        CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &adminSID, &dwSize);
 
        if ( *pElevationType == TokenElevationTypeLimited )
        {
            HANDLE hUnfilteredToken = NULL;
             
            // 연결된 토큰의 핸들을 얻는다.
            GetTokenInformation(hToken, TokenLinkedToken, (void *)&hUnfilteredToken, sizeof(HANDLE), &dwSize);
 
            // 원래의 토큰이 관리자의 SID를 포함하고 있는지 여부를 확인한다.
            if ( CheckTokenMembership(hUnfilteredToken, &adminSID, pIsAdmin) )
                bResult = TRUE;
             
            CloseHandle(hUnfilteredToken);
        }
        else
        {
            *pIsAdmin = IsUserAnAdmin();
            bResult = TRUE;
        }
    }
 
    CloseHandle(hToken);
    return bResult;
} 

////////////////////////////////////////////////////////////////////////////////
// check permission of Administrator
////////////////////////////////////////////////////////////////////////////////

/* http://www.cplusplus.com/forum/windows/101207/ 
 * https://www.codeproject.com/Articles/18509/Add-a-UAC-shield-to-a-button-when-elevation-is-req
 * 
 * return value 
 *   - TRUE  : Administrator 권한 획득
 *   - FALSE : Administrator 권한 실패
 */
BOOL CUtilAccount::RunByAdministrator()
{
	BOOL bAlreadyRunningAsAdministrator = FALSE;
	try {
#if 1 // 관리자 그룹에 속해 있는지 확인한다.
        bAlreadyRunningAsAdministrator = IsCurrentGroupLocalAdministrators();
#else
        bAlreadyRunningAsAdministrator = IsCurrentUserLocalAdministrator();
#endif
	} catch(...) {
    	_asm nop
	}

	if( bAlreadyRunningAsAdministrator == FALSE ) {
		char szPath[MAX_PATH];
		if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath))) {
			SHELLEXECUTEINFO sei = { sizeof(sei) };
			
			sei.lpVerb = "runas";
			sei.lpFile = szPath;
			sei.hwnd = NULL;
			sei.nShow = SW_NORMAL;

			if (!ShellExecuteEx(&sei)) {
				DWORD dwError = GetLastError();
				switch (dwError) {
					case ERROR_WRONG_PASSWORD: {
						CreateThread(0, 0, (LPTHREAD_START_ROUTINE)RunByAdministrator, 0, 0, 0);
					}
					break;
					case ERROR_CANCELLED: {
						return FALSE;
					}
					break;
				}
       	    }
		}
	} else {
		///Code
	}

	return TRUE;
}

/* http://www.cplusplus.com/forum/windows/101207/ */
BOOL CUtilAccount::IsRunAsAdministrator()
{
    BOOL fIsRunAsAdmin = FALSE;
    DWORD dwError = ERROR_SUCCESS;
    PSID pAdministratorsGroup = NULL;

    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    if (!AllocateAndInitializeSid(
        &NtAuthority, 
        2, 
        SECURITY_BUILTIN_DOMAIN_RID, 
        DOMAIN_ALIAS_RID_ADMINS, 
        0, 0, 0, 0, 0, 0, 
        &pAdministratorsGroup))
    {
        dwError = GetLastError();
        goto Cleanup;
    }

    if (!CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin))
    {
        dwError = GetLastError();
        goto Cleanup;
    }

Cleanup:

    if (pAdministratorsGroup)
    {
        FreeSid(pAdministratorsGroup);
        pAdministratorsGroup = NULL;
    }

    if (ERROR_SUCCESS != dwError)
    {
        throw dwError;
    }

    return fIsRunAsAdmin;
}

BOOL CUtilAccount::IsCurrentGroupLocalAdministrators()
{
    CString strAccountID = CUtilAccount::GetAccountID();
    CMyStringArray saGroupName = CUtilAccount::GetAccountGroup(strAccountID);
    for (int i = 0; i < saGroupName.GetCount(); i++) {
        CString strGroupName = saGroupName.GetAt(i);
        if (strGroupName == GROUP_ADMINISTRATORS) {
            return TRUE;
        }
    }

    return FALSE;
}

/* https://www.codeproject.com/Questions/544981/IsplusProcessplusRunningplusasplusAdministrator */
BOOL CUtilAccount::IsCurrentUserLocalAdministrator()
{
    BOOL   fReturn         = FALSE;
    DWORD  dwStatus        = 0;
    DWORD  dwAccessMask    = 0;
    DWORD  dwAccessDesired = 0;
    DWORD  dwACLSize       = 0;
    DWORD  dwStructureSize = sizeof(PRIVILEGE_SET);
    PACL   pACL            = NULL;
    PSID   psidAdmin       = NULL;
 
    HANDLE hToken              = NULL;
    HANDLE hImpersonationToken = NULL;
 
    PRIVILEGE_SET   ps = {0};
    GENERIC_MAPPING GenericMapping = {0};
 
    PSECURITY_DESCRIPTOR     psdAdmin           = NULL;
    SID_IDENTIFIER_AUTHORITY SystemSidAuthority = SECURITY_NT_AUTHORITY;
 
    // Determine if the current thread is running as a user that is a member 
    // of the local admins group.  To do this, create a security descriptor 
    // that has a DACL which has an ACE that allows only local administrators 
    // access.  Then, call AccessCheck with the current thread's token and
    // the security descriptor.  It will say whether the user could access an
    // object if it had that security descriptor.  Note: you do not need to
    // actually create the object.  Just checking access against the
    // security descriptor alone will be sufficient.
 
    const DWORD ACCESS_READ  = 1UL;
    const DWORD ACCESS_WRITE = 2UL;
 
    __try
    {
        // AccessCheck() requires an impersonation token.  We first get a 
        // primary token and then create a duplicate impersonation token.
        // The impersonation token is not actually assigned to the thread, but
        // is used in the call to AccessCheck.  Thus, this function itself never
        // impersonates, but does use the identity of the thread.  If the thread
        // was impersonating already, this function uses that impersonation 
        // context.
        if (!OpenThreadToken(GetCurrentThread(), TOKEN_DUPLICATE|TOKEN_QUERY, TRUE, &hToken))
        {
            if (GetLastError() != ERROR_NO_TOKEN)
                __leave;
 
            if (!OpenProcessToken(GetCurrentProcess(), 
                TOKEN_DUPLICATE|TOKEN_QUERY, &hToken))
                __leave;
        }
 
        if (!DuplicateToken (hToken, SecurityImpersonation, &hImpersonationToken))
            __leave;
 
        // Create the binary representation of the well-known SID that
        // represents the local administrators group.  Then create the 
        // security descriptor and DACL with an ACE that allows only local
        // admins access.  After that, perform the access check.  This will
        // determine whether the current user is a local admin.
        if (!AllocateAndInitializeSid(&SystemSidAuthority, 2,
            SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
            0, 0, 0, 0, 0, 0, &psidAdmin))
            __leave;
 
        psdAdmin = LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
        if (psdAdmin == NULL)
            __leave;
 
        if (!InitializeSecurityDescriptor(psdAdmin, SECURITY_DESCRIPTOR_REVISION))
            __leave;
 
        // Compute size needed for the ACL.
        dwACLSize = sizeof(ACL) + sizeof(ACCESS_ALLOWED_ACE) +
            GetLengthSid(psidAdmin) - sizeof(DWORD);
 
        pACL = (PACL)LocalAlloc(LPTR, dwACLSize);
        if (pACL == NULL)
            __leave;
 
        if (!InitializeAcl(pACL, dwACLSize, ACL_REVISION2))
            __leave;
 
        dwAccessMask= ACCESS_READ | ACCESS_WRITE;
 
        if (!AddAccessAllowedAce(pACL, ACL_REVISION2, dwAccessMask, psidAdmin))
            __leave;
 
        if (!SetSecurityDescriptorDacl(psdAdmin, TRUE, pACL, FALSE))
            __leave;
 
        // AccessCheck validates a security descriptor somewhat; set the group
        // and owner so that enough of the security descriptor is filled out to
        // make AccessCheck happy.
 
        SetSecurityDescriptorGroup(psdAdmin, psidAdmin, FALSE);
        SetSecurityDescriptorOwner(psdAdmin, psidAdmin, FALSE);
 
        if (!IsValidSecurityDescriptor(psdAdmin))
            __leave;
 
        dwAccessDesired = ACCESS_READ;
 
        // Initialize GenericMapping structure even though you
        // do not use generic rights.
        GenericMapping.GenericRead    = ACCESS_READ;
        GenericMapping.GenericWrite   = ACCESS_WRITE;
        GenericMapping.GenericExecute = 0;
        GenericMapping.GenericAll     = ACCESS_READ | ACCESS_WRITE;
 
        if (!AccessCheck(psdAdmin, hImpersonationToken, dwAccessDesired,
            &GenericMapping, &ps, &dwStructureSize, &dwStatus,
            &fReturn))
        {
            fReturn = FALSE;
            __leave;
        }
    }
    __finally
    {
        // Clean up.
        if (pACL)
            LocalFree(pACL);
        if (psdAdmin)
            LocalFree(psdAdmin);
        if (psidAdmin)
            FreeSid(psidAdmin);
        if (hImpersonationToken)
            CloseHandle (hImpersonationToken);
        if (hToken)
            CloseHandle (hToken);
    }
 
    return fReturn;
}

////////////////////////////////////////////////////////////////////////////////
// account id/group/auth
////////////////////////////////////////////////////////////////////////////////

#pragma comment(lib, "netapi32.lib")
#include <stdio.h>
#include <assert.h>
#include <windows.h>
#include <lm.h>

CString CUtilAccount::GetAccountID()
{
    char   szAccountID[128];
    memset( szAccountID, 0, sizeof(szAccountID));
    DWORD dwSize =  sizeof(szAccountID);
    CString strAccount = "";

    if( ::GetUserName( szAccountID, &dwSize ) == TRUE )  {
        strAccount.SetString(szAccountID);
    }
    return strAccount;
}

CMyStringArray CUtilAccount::GetAccountGroup( CString strAccountID )
{
    static CMyStringArray saGroupName;
    saGroupName.RemoveAll();

    LPLOCALGROUP_USERS_INFO_0 pBuf = NULL;
    DWORD dwLevel = 0;
    DWORD dwFlags = LG_INCLUDE_INDIRECT ;
    DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
    DWORD dwEntriesRead = 0;
    DWORD dwTotalEntries = 0;
    NET_API_STATUS nStatus;

    BSTR bstrString       = strAccountID.AllocSysString();
    LPWSTR pwstrAccoundID = (LPWSTR)bstrString;
    {
        nStatus = ::NetUserGetLocalGroups( NULL,           // NULL is localcomputer
                                           pwstrAccoundID, // L"Administrator",
                                           dwLevel,
                                           dwFlags,
                                           (LPBYTE *) &pBuf,
                                           dwPrefMaxLen,
                                           &dwEntriesRead,
                                           &dwTotalEntries);
    }
    SysFreeString(bstrString);

    if (nStatus == NERR_Success) {
        LPLOCALGROUP_USERS_INFO_0 pTmpBuf;
        DWORD i;
        DWORD dwTotalCount = 0;
        if ((pTmpBuf = pBuf) != NULL) {
            //
            // Loop through the entries and 
            //  print the names of the local groups 
            //  to which the user belongs. 
            //
            for (i = 0; i < dwEntriesRead; i++) {
                assert(pTmpBuf != NULL);
                if (pTmpBuf == NULL) {
                    //-AfxMessageBox( "An access violation has occurred" );
                    break;
                }
                CStringW strwGroupName(pTmpBuf->lgrui0_name);
                CStringA straGroupName = CW2A(strwGroupName, CP_UTF8);
                saGroupName.Add(straGroupName);

                pTmpBuf++;
                dwTotalCount++;
            }
        }
        //
        // If all available entries were
        //  not enumerated, print the number actually 
        //  enumerated and the total number available.
        //
        if (dwEntriesRead < dwTotalEntries) {
            CString strErr;
            strErr.Format( "Total entries: %d", dwTotalEntries );
            //-AfxMessageBox( strErr );
        }
        //
        // Otherwise, just print the total.
        //
        CString strMsg;
        strMsg.Format( "Entries enumerated: %d", dwTotalCount );
        //-AfxMessageBox( strMsg );
    } else {
        CString strStatus = "";
        switch( nStatus ) {
            case ERROR_ACCESS_DENIED :          strStatus = "ERROR_ACCESS_DENIED";      break;
            case ERROR_INVALID_LEVEL :          strStatus = "ERROR_INVALID_LEVEL";      break;
            case ERROR_INVALID_PARAMETER :      strStatus = "ERROR_INVALID_PARAMETER";  break;
            case ERROR_MORE_DATA :              strStatus = "ERROR_MORE_DATA";          break;
            case ERROR_NOT_ENOUGH_MEMORY :      strStatus = "ERROR_NOT_ENOUGH_MEMORY";  break;
            case NERR_DCNotFound :              strStatus = "NERR_DCNotFound";          break;
            case NERR_UserNotFound :            strStatus = "NERR_UserNotFound";        break;
            case RPC_S_SERVER_UNAVAILABLE :     strStatus = "RPC_S_SERVER_UNAVAILABLE"; break;
        }

        CString strErr;
        strErr.Format( "A system error has occurred : %s", strStatus );
        //-AfxMessageBox( strErr );
    }
    //
    // Free the allocated memory.
    //
    if (pBuf != NULL) {
        ::NetApiBufferFree(pBuf);
    }

    return saGroupName;
}

CString CUtilAccount::GetAccountAuth()
{
   return "";
}
