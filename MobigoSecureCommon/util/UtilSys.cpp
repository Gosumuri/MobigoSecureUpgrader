////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "utilsys.h"
#include "UtilRegKey.h"
#include "UtilOS.h"

#include <ShellScalingAPI.h>
#include <IPHlpApi.h>
#pragma comment(lib, "IPHLPAPI.lib")

#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>
#include <Psapi.h>
#include <WinInet.h> //��Ʈ��ũ Ȯ���� ���� �Լ� ��� ���� �߰�
#include <wlanapi.h>
#pragma comment(lib, "wlanapi.lib")
////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

#define CRITERION_MONITOR_WIDTH         510
#define CRITERION_MONITOR_HEIGHT        290

#define CRITERION_DPI_WIDTH             96
#define CRITERION_DPI_HEIGHT            96

////////////////////////////////////////////////////////////////////////////////
// static member
////////////////////////////////////////////////////////////////////////////////

CRect                           CUtilSys::m_rectMonitorResolution( 0, 0, 0, 0);

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CUtilSys::CUtilSys(void)
{
    
}

CUtilSys::~CUtilSys(void)
{
}

////////////////////////////////////////////////////////////////////////////////
// check network connection
// https://stackoverflow.com/questions/5917304/how-do-i-detect-a-disabled-network-interface-connection-from-a-windows-applicati
// http://egloos.zum.com/itbaby/v/4742980 <- 18.01.31 ���� ����� ���� 
////////////////////////////////////////////////////////////////////////////////

BOOL CUtilSys::IsNetworkConnection()
{
    DWORD dwConnectionTypes;
    
	if(::InternetGetConnectedState(&dwConnectionTypes, 0)) { // ���������� �˻��
		if((dwConnectionTypes & INTERNET_CONNECTION_MODEM) != 0){
			//-AfxMessageBox("Modem���� ���ͳ� �����...");
        }
        if((dwConnectionTypes & INTERNET_CONNECTION_LAN) != 0){
			//-AfxMessageBox("LAN���� ���ͳ� �����...");
        }
        if((dwConnectionTypes & INTERNET_CONNECTION_PROXY) != 0){
			//-AfxMessageBox("Proxy�� ���ͳ� �����...");
        }
        if((dwConnectionTypes & INTERNET_CONNECTION_OFFLINE) != 0){ //���������� ��� ���� ���ͳ� Ȯ��
			//-AfxMessageBox("��������");
            return IsWirelessNetworkConnection();
        }
        if((dwConnectionTypes & INTERNET_CONNECTION_CONFIGURED) != 0){
			//-AfxMessageBox("���ͳ� ������ �����Ǿ���");
        }
        return TRUE;
	} else {
		//-AfxMessageBox("�˻��� �� �����ϴ�");
    }

    return FALSE;
}
////////////////////////////////////////////////////////////////////////////////
// check Wireless network connection
// https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms706759(v=vs.85).aspx
// IsNetworkConnection()���� ���Ǵ� �Լ�(InternetGetConnectedState)���� ���� ���ͳ� ���¸� Ȯ�� ���� ���Ѵٴ� �̾߱Ⱑ �־� �߰��ص�..
// ���� �̻�� �� ���� ���� 
////////////////////////////////////////////////////////////////////////////////

BOOL CUtilSys::IsWirelessNetworkConnection() //���� ���ͳ� 
{
    HANDLE hClient = NULL;
    DWORD dwMaxClient = 2;      // ���� ����Ʈ�� ���������� 1: XP ������ 3, 2: vista and server 2008
    DWORD dwCurVersion = 0;
    DWORD dwResult = 0;
    DWORD dwRetVal = 0;
    
    //int iRet = 0;
    //WCHAR GuidString[39] = {0};
    CString strError ="";

	//���� ���ͳ� ������ ������� ����
    PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
    PWLAN_INTERFACE_INFO      pIfInfo = NULL;

	// ���� ���ͳ� open  ( 1062 ���� : "WLAN AutoConfig" was not running. )
	dwResult = ::WlanOpenHandle(
		dwMaxClient,    //The highest version of the WLAN API that the client supports.
		NULL,       	//Reserved for future use. Must be set to NULL.
		&dwCurVersion,  //The version of the WLAN API that will be used in this session. This value is usually the highest version supported by both the client and server.
		&hClient); 	    //A handle for the client to use in this session. This handle is used by other functions throughout the session.
		
    if (dwResult != ERROR_SUCCESS) {
        strError.Format("WlanOpenHandle failed with error: %u\n", dwResult);
        //AfxMessageBox(strError);
        return FALSE;
    }
	
	//WlanEnumInterfaces : ���� ���� ��ǻ�Ϳ��� Ȱ��ȭ�� ��� ���� LAN�������̽��� �����մϴ�.
	//If the function succeeds, the return value is ERROR_SUCCESS.
	//If the function fails, the return value may be one of the following return codes.	
     dwResult = ::WlanEnumInterfaces(
	  hClient,		//The client's session handle, obtained by a previous call to the WlanOpenHandle function.
	  NULL,		    //Reserved for future use. This parameter must be set to NULL.
	   &pIfList);   //A pointer to storage for a pointer to receive the returned list of wireless LAN interfaces in a WLAN_INTERFACE_INFO_LIST structure.

    if (dwResult != ERROR_SUCCESS) {
        strError.Format("WlanEnumInterfaces failed with error: %u\n", dwResult);
       //  AfxMessageBox("WlanEnumInterfaces failed!!!!");
        return FALSE;
        // You can use FormatMessage here to find out why the function failed
    } else {
        /* �α׿�
        strError.Format("Num Entries: %lu\n", pIfList->dwNumberOfItems);
        AfxMessageBox(strError);
        strError.Format("Current Index: %lu\n", pIfList->dwIndex);
        AfxMessageBox(strError);
        */
        for (int nItemNum = 0; nItemNum < (int) pIfList -> dwNumberOfItems; nItemNum++) {
            pIfInfo = (WLAN_INTERFACE_INFO *) &pIfList->InterfaceInfo[nItemNum];

			/* �α׿�, ���� �ҽ��� ����������.. �ּ�ó��
            strError.Format("  Interface Index[%u]:\t %lu\n", nItemNum, nItemNum);
            AfxMessageBox(strError);
            iRet = StringFromGUID2(pIfInfo->InterfaceGuid, (LPOLESTR) &GuidString, sizeof(GuidString)/sizeof(*GuidString)); 
            
            if (iRet == 0){
                wprintf(L"StringFromGUID2 failed\n");
            }else {
                wprintf(L"  InterfaceGUID[%d]: %ws\n",i, GuidString);
            }    
            wprintf(L"  Interface Description[%d]: %ws", i, pIfInfo->strInterfaceDescription);
            wprintf(L"\n");
            wprintf(L"  Interface State[%d]:\t ", i);
            */
            switch (pIfInfo->isState) {

            case wlan_interface_state_connected:
                AfxMessageBox("Connected!!!!");
                return TRUE;
                break;

            case wlan_interface_state_disconnecting:
                AfxMessageBox("Disconnecting!!!!");
                break;

            case wlan_interface_state_disconnected:
                AfxMessageBox("Not connected!!!!");
                break;
            }
        }
    }
    return FALSE;
}


////////////////////////////////////////////////////////////////////////////////
// create short cut
// https://www.codeproject.com/Articles/5127/How-to-create-and-resolve-a-shortcut
////////////////////////////////////////////////////////////////////////////////

/**********************************************************************
* Function......: CreateShortcut
* Parameters....: lpszFileName - string that specifies a valid file name
*          lpszDesc - string that specifies a description for a 
                             shortcut
*          lpszShortcutPath - string that specifies a path and 
                                     file name of a shortcut
* Returns.......: S_OK on success, error code on failure
* Description...: Creates a Shell link object (shortcut)
**********************************************************************/

HRESULT CUtilSys::CreateShortcut(   /*in*/ LPCTSTR lpszFileName, 
                                    /*in*/ LPCTSTR lpszDesc, 
                                    /*in*/ LPCTSTR lpszShortcutPath)
{
    HRESULT hRes = E_FAIL;
    DWORD dwRet = 0;
    CComPtr<IShellLink> ipShellLink;
        // buffer that receives the null-terminated string 
        // for the drive and path
    TCHAR szPath[MAX_PATH];    
        // buffer that receives the address of the final 
        //file name component in the path
    LPTSTR lpszFilePart;    
    WCHAR wszTemp[MAX_PATH];
        
    // Retrieve the full path and file name of a specified file
    dwRet = GetFullPathName(lpszFileName, 
                       sizeof(szPath) / sizeof(TCHAR), 
                       szPath, &lpszFilePart);
    if (!dwRet)                                        
        return hRes;

    // Get a pointer to the IShellLink interface
    hRes = CoCreateInstance(CLSID_ShellLink,
                            NULL, 
                            CLSCTX_INPROC_SERVER,
                            IID_IShellLink,
                            (void**)&ipShellLink);

    if (SUCCEEDED(hRes))
    {
        // Get a pointer to the IPersistFile interface
        CComQIPtr<IPersistFile> ipPersistFile(ipShellLink);

        // Set the path to the shortcut target and add the description
        hRes = ipShellLink->SetPath(szPath);
        if (FAILED(hRes))
            return hRes;

        hRes = ipShellLink->SetDescription(lpszDesc);
        if (FAILED(hRes))
            return hRes;

        // IPersistFile is using LPCOLESTR, so make sure 
                // that the string is Unicode
#if !defined _UNICODE
        MultiByteToWideChar(CP_ACP, 0, 
                       lpszShortcutPath, -1, wszTemp, MAX_PATH);
#else
        wcsncpy(wszTemp, lpszShortcutPath, MAX_PATH);
#endif

        // Write the shortcut to disk
        hRes = ipPersistFile->Save(wszTemp, TRUE);
    }

    return hRes;
}

////////////////////////////////////////////////////////////////////////////////
// resolve short cut( ���۾��� )
// https://www.codeproject.com/Articles/5127/How-to-create-and-resolve-a-shortcut
////////////////////////////////////////////////////////////////////////////////

/*********************************************************************
* Function......: ResolveShortcut
* Parameters....: lpszShortcutPath - string that specifies a path 
                                     and file name of a shortcut
*          lpszFilePath - string that will contain a file name
* Returns.......: S_OK on success, error code on failure
* Description...: Resolves a Shell link object (shortcut)
*********************************************************************/

HRESULT CUtilSys::ResolveShortcut(  /*in*/ LPCTSTR lpszShortcutPath,
                                    /*out*/ LPTSTR lpszFilePath)
{
    HRESULT hRes = E_FAIL;
    CComPtr<IShellLink> ipShellLink;
        // buffer that receives the null-terminated string 
        // for the drive and path
    TCHAR szPath[MAX_PATH];     
        // buffer that receives the null-terminated 
        // string for the description
    TCHAR szDesc[MAX_PATH]; 
        // structure that receives the information about the shortcut
    WIN32_FIND_DATA wfd;    
    WCHAR wszTemp[MAX_PATH];

    lpszFilePath[0] = '\0';

    // Get a pointer to the IShellLink interface
    hRes = CoCreateInstance(CLSID_ShellLink,
                            NULL, 
                            CLSCTX_INPROC_SERVER,
                            IID_IShellLink,
                            (void**)&ipShellLink); 

    if (SUCCEEDED(hRes)) 
    { 
        // Get a pointer to the IPersistFile interface
        CComQIPtr<IPersistFile> ipPersistFile(ipShellLink);

        // IPersistFile is using LPCOLESTR, 
                // so make sure that the string is Unicode
#if !defined _UNICODE
        MultiByteToWideChar(CP_ACP, 0, lpszShortcutPath,
                                       -1, wszTemp, MAX_PATH);
#else
        wcsncpy(wszTemp, lpszShortcutPath, MAX_PATH);
#endif

        // Open the shortcut file and initialize it from its contents
        hRes = ipPersistFile->Load(wszTemp, STGM_READ); 
        if (SUCCEEDED(hRes)) 
        {
            // Try to find the target of a shortcut, 
                        // even if it has been moved or renamed
            hRes = ipShellLink->Resolve(NULL, SLR_UPDATE); 
            if (SUCCEEDED(hRes)) 
            {
                // Get the path to the shortcut target
                hRes = ipShellLink->GetPath(szPath, 
                                     MAX_PATH, &wfd, SLGP_RAWPATH); 
                if (FAILED(hRes))
                    return hRes;

                // Get the description of the target
                hRes = ipShellLink->GetDescription(szDesc,
                                             MAX_PATH); 
                if (FAILED(hRes))
                    return hRes;

                lstrcpyn(lpszFilePath, szPath, MAX_PATH); 
            } 
        } 
    } 

    return hRes;
}

////////////////////////////////////////////////////////////////////////////////
// desktop short cut
////////////////////////////////////////////////////////////////////////////////

/*
    function name: CreateDesktopShortcut()
    Description  : ����ȭ�鿡 ������ �� �������� ������ �����Ѵ�.
        - ó�� ����  TRUE
        - ó�� ����  FALSE
    parameter  : 
        - strFilePath    - ������ ��� exe�� Ǯ �н�
        - strTitle       - ������ ����
*/

BOOL CUtilSys::CreateDesktopShortcut(   LPTSTR      pszFilePath, 
                                        LPTSTR      pszTitle )
{
    TCHAR szDesktopPath[MAX_PATH];
    memset( szDesktopPath, 0x00, sizeof(szDesktopPath) );
    // ����ȭ�� �������� ����� ��� ����
    if( FAILED( ::SHGetFolderPath(NULL, CSIDL_DESKTOP , NULL, 0, szDesktopPath) ) ) {
        return FALSE;
    }
    CString strShortcutPath = "";
    strShortcutPath.Format( "%s\\%s.lnk", szDesktopPath, pszTitle );

    return CreateShortcut(   /*in*/ pszFilePath, 
                             /*in*/ pszTitle, 
                             /*in*/ strShortcutPath);
}


////////////////////////////////////////////////////////////////////////////////
// disk space
// ( http://cinema4dr12.tistory.com/632 )
////////////////////////////////////////////////////////////////////////////////

// ���� : Giga Byte
long CUtilSys::GetDiskTotalSpace( CString strDisk, BOOL bGiga /* TRUE==GIGA, FALSE==MEGA */ )
{
    ULARGE_INTEGER  ulnAvail;
    ULARGE_INTEGER  ulnTotal; 
    ULARGE_INTEGER  ulnFree ;
    ::GetDiskFreeSpaceEx( strDisk, &ulnAvail, &ulnTotal, &ulnFree);

    long lAvail = -1;
    long lTotal = -1;
    long lFree  = -1;

    if( bGiga == TRUE ) {
        lAvail = (long)(ulnAvail.QuadPart>>30);
        lTotal = (long)(ulnTotal.QuadPart>>30);
        lFree  = (long)(ulnFree .QuadPart>>30);
    } else {
        lAvail = (long)(ulnAvail.QuadPart>>20);
        lTotal = (long)(ulnTotal.QuadPart>>20);
        lFree  = (long)(ulnFree .QuadPart>>20);
    }

    return lTotal;
}

// ���� : Giga Byte
long CUtilSys::GetDiskUsedSpace(  CString strDisk, BOOL bGiga /* TRUE==GIGA, FALSE==MEGA */ )
{
    ULARGE_INTEGER  ulnAvail;
    ULARGE_INTEGER  ulnTotal; 
    ULARGE_INTEGER  ulnFree ;
    ::GetDiskFreeSpaceEx( strDisk, &ulnAvail, &ulnTotal, &ulnFree);

    long lAvail = -1;
    long lTotal = -1;
    long lFree  = -1;

    if( bGiga == TRUE ) {
        lAvail = (long)(ulnAvail.QuadPart>>30);
        lTotal = (long)(ulnTotal.QuadPart>>30);
        lFree  = (long)(ulnFree .QuadPart>>30);
    } else {
        lAvail = (long)(ulnAvail.QuadPart>>20);
        lTotal = (long)(ulnTotal.QuadPart>>20);
        lFree  = (long)(ulnFree .QuadPart>>20);
    }

    return lTotal - lFree;
}

// ���� : Giga Byte
long CUtilSys::GetDiskFreeSpace(  CString strDisk, BOOL bGiga /* TRUE==GIGA, FALSE==MEGA */ )
{
    ULARGE_INTEGER  ulnAvail;
    ULARGE_INTEGER  ulnTotal; 
    ULARGE_INTEGER  ulnFree ;
    ::GetDiskFreeSpaceEx( strDisk, &ulnAvail, &ulnTotal, &ulnFree);

    long lAvail = -1;
    long lTotal = -1;
    long lFree  = -1;

    if( bGiga == TRUE ) {
        lAvail = (long)(ulnAvail.QuadPart>>30);
        lTotal = (long)(ulnTotal.QuadPart>>30);
        lFree  = (long)(ulnFree .QuadPart>>30);
    } else {
        lAvail = (long)(ulnAvail.QuadPart>>20);
        lTotal = (long)(ulnTotal.QuadPart>>20);
        lFree  = (long)(ulnFree .QuadPart>>20);
    }

    return lFree;
}

////////////////////////////////////////////////////////////////////////////////
// ie version
// ( https://www.arclab.com/en/kb/cppmfc/get-internet-explorer-version-from-registry.html )
////////////////////////////////////////////////////////////////////////////////

CString CUtilSys::GetIEVersion()
{
    CString strIEVersion = "";
    BOOL b = CUtilRegKey::GetValueStr(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Internet Explorer"), _T("svcVersion"), strIEVersion );

    if( b == FALSE || strIEVersion.IsEmpty() == TRUE ) {

        b = CUtilRegKey::GetValueStr(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Internet Explorer"), _T("Version"), strIEVersion );

    }     
    return strIEVersion;
}

////////////////////////////////////////////////////////////////////////////////
// computer name
// ( http://egloos.zum.com/mindol/v/2048894 )
////////////////////////////////////////////////////////////////////////////////

CString CUtilSys::GetComputerName()
{
    DWORD nSize =  MAX_COMPUTERNAME_LENGTH + 1;
    char szBuffer[MAX_COMPUTERNAME_LENGTH + 1];
    memset( szBuffer, 0, sizeof(szBuffer));
    CString strComputerName = "" ;
    if( ::GetComputerName(szBuffer,&nSize) == TRUE ) {
        strComputerName.SetString(szBuffer);
    }
    return strComputerName;
}

////////////////////////////////////////////////////////////////////////////////
// System Information 
//  - Hard Disk Volume Serial 
//  - GateWay IP
//  - IPv4
//  - DNS
//  - MAC
////////////////////////////////////////////////////////////////////////////////

CString CUtilSys::GetVolumeSerial()
{
    TCHAR szVolNameBuffer[MAX_PATH] = { 0 };
    DWORD dwVolSerNum = 0;
    DWORD dwMaxCompLength = 0;
    DWORD dwFileSysFlag = 0;
    TCHAR szFSName[32] = { 0 };

    TCHAR SystemDir[_MAX_PATH] = { 0 };
    TCHAR drive[_MAX_PATH] = { 0 };
    TCHAR dir[_MAX_DIR] = { 0 };
    TCHAR fname[_MAX_FNAME] = { 0 };
    TCHAR ext[_MAX_EXT] = { 0 };

    TCHAR Buffer[MAX_PATH] = { '\0', };
    TCHAR tBuffer[MAX_PATH] = { '\0', };
    //LPTSTR pBuffer;
    GetSystemDirectory(SystemDir, MAX_PATH);

    //GetWindowsDirectory(WinBuffer, MAX_PATH);

    _tsplitpath_s(SystemDir, drive, _MAX_PATH, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT);
    if (drive[0] == _T('\0'))
        _tcscpy_s(drive, _countof(drive), _T("C:\\"));
    else
        _tcscat_s(drive, _countof(drive), _T("\\"));

    if (GetVolumeInformation(drive, szVolNameBuffer, MAX_PATH, &dwVolSerNum, &dwMaxCompLength, &dwFileSysFlag, szFSName, 32))
    {

        LPTSTR pBuffer = tBuffer;
        _stprintf_s(tBuffer, MAX_PATH, _T("%X"), dwVolSerNum);
        
    }
    return CString(tBuffer);
}
CString CUtilSys::GetGateWayIPAddress()
{
    CString strGateWayIP = "";
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD errValue = 0;
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
    
    if (!pAdapterInfo)
        return "";
    
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) 
    {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
        if (!pAdapterInfo) 
            return "";;
    }
    CString info;
    if ((errValue = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
    {
        pAdapter = pAdapterInfo;
        while (pAdapter)
        {
            CString type;

            if (pAdapter->Type == IF_TYPE_IEEE80211 || pAdapter->Type == MIB_IF_TYPE_ETHERNET)
            {

                CString strCurGatewayIP = CString(pAdapter->GatewayList.IpAddress.String);
                
                if (strGateWayIP == "" && strCurGatewayIP.Compare("0.0.0.0") !=0 )
                {
                    strGateWayIP = strCurGatewayIP;
                }
                
                pAdapter = pAdapter->Next;
            }
        }
    }


    if (pAdapterInfo)
        free(pAdapterInfo);
    
    return strGateWayIP;
}
CString CUtilSys::GetIPv4IPAddress()
{
    CString strIPv4IP = "";
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD errValue = 0;
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));

    if (!pAdapterInfo)
        return "";

    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
        if (!pAdapterInfo)
            return "";;
    }
    CString info;
    if ((errValue = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
    {
        pAdapter = pAdapterInfo;
        while (pAdapter)
        {
            CString type;

            if (pAdapter->Type == IF_TYPE_IEEE80211 || pAdapter->Type == MIB_IF_TYPE_ETHERNET)
            {

                CString strCurIP = CString(pAdapter->IpAddressList.IpAddress.String);

                if (strIPv4IP == "" && strCurIP.Compare("0.0.0.0") != 0)
                {
                    strIPv4IP = strCurIP;
                }

                pAdapter = pAdapter->Next;
            }
        }
    }
    if (pAdapterInfo)
        free(pAdapterInfo);

    return strIPv4IP;
}
CString CUtilSys::GetDNSAddress()
{
    CString strDNSIP = "";
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    IP_PER_ADAPTER_INFO* pPerAdapt = NULL;
    ULONG ulLen = sizeof(IP_PER_ADAPTER_INFO);
    DWORD errValue = 0;
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));

    if (!pAdapterInfo)
        return "";

    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
        if (!pAdapterInfo)
            return "";;
    }
    
    CString info;
    if ((errValue = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
        pAdapter = pAdapterInfo;
        while (pAdapter) {
            int err = GetPerAdapterInfo(pAdapter->Index, pPerAdapt, &ulLen);
            CString type;
            if (ERROR_BUFFER_OVERFLOW == err) {
                pPerAdapt = (IP_PER_ADAPTER_INFO*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ulLen);
                err = GetPerAdapterInfo(pAdapter->Index, pPerAdapt, &ulLen);
            }

            if (pAdapter->Type == IF_TYPE_IEEE80211 || pAdapter->Type == MIB_IF_TYPE_ETHERNET) {
                if (err == ERROR_SUCCESS) {
                    IP_ADDR_STRING* pDnsList = &(pPerAdapt->DnsServerList);
                    CString strCurDNSIP = pDnsList->IpAddress.String;

                    if (strDNSIP == "" && strCurDNSIP.Compare("0.0.0.0") != 0) {
                        strDNSIP = strCurDNSIP;
                    }
                }
                pAdapter = pAdapter->Next;
            }
        }
    }

    if (pAdapterInfo) {
        free(pAdapterInfo);
    }

    return strDNSIP;
}

CString CUtilSys::GetMACAddress()
{
    CString strGateWay = "";
    CString strMACAddress = "00-00-00-00-00-00";
    IP_ADAPTER_INFO ipAdapterInfo[5];
    DWORD dwBuflen = sizeof(ipAdapterInfo);

    DWORD dwStatus = GetAdaptersInfo(ipAdapterInfo, &dwBuflen);
    if (dwStatus != ERROR_SUCCESS) {
        strMACAddress.Format(_T("Error for GetAdaptersInfo : %d"), dwStatus);
        return _T("");
    }

    PIP_ADAPTER_INFO pIpAdapterInfo = ipAdapterInfo;
    do{
        strGateWay = (CString)pIpAdapterInfo->GatewayList.IpAddress.String;
        if (strGateWay[0] == '0') {
            pIpAdapterInfo = pIpAdapterInfo->Next;
        } else {
            strMACAddress.Format(_T("%02X%02X%02X%02X%02X%02X"),
                pIpAdapterInfo->Address[0],
                pIpAdapterInfo->Address[1],
                pIpAdapterInfo->Address[2],
                pIpAdapterInfo->Address[3],
                pIpAdapterInfo->Address[4],
                pIpAdapterInfo->Address[5]
                );
            break;
        }
    } while (pIpAdapterInfo);

    return strMACAddress;
}

CString CUtilSys::GetMACAddress( CString address )
{
    ULONG uMacAddr[2];
    ULONG len = 6;
    CString str = "";
    BYTE *bPhysAddr;

    memset(&uMacAddr, 0xff, sizeof(uMacAddr));
    
    int result = SendARP(inet_addr(address), 0, uMacAddr, &len);

    if (result == NOERROR) {
        bPhysAddr = (BYTE *)& uMacAddr;
        for (ULONG i = 0; i < len; i++) {
            CString strTemp;
            strTemp.Format("%02X", bPhysAddr[i]);
            str += strTemp;
        }
    }

    return str;
}

////////////////////////////////////////////////////////////////////////////////
// ���� �����, ���� �ִ� tray icon ���ֱ� 
// - http://dduk02.tistory.com/entry/Ʈ���̾�����-�ܻ�-���ֱ�
// - https://m.blog.naver.com/kil1rcn/30150374105 
////////////////////////////////////////////////////////////////////////////////

struct TRAYDATA
{
    HWND hwnd;
    UINT uID;
    UINT uCallbackMessage;
    DWORD Reserved[2];
    HICON hIcon;
};

static HWND FindTrayToolbarWindow()
{
    HWND hWnd_ToolbarWindow32 = NULL;
    HWND hWnd_ShellTrayWnd;
    hWnd_ShellTrayWnd = ::FindWindow(_T("Shell_TrayWnd"), NULL);
    if(hWnd_ShellTrayWnd) {
        HWND hWnd_TrayNotifyWnd = ::FindWindowEx(hWnd_ShellTrayWnd,NULL,_T("TrayNotifyWnd"), NULL);
        if(hWnd_TrayNotifyWnd) {
            HWND hWnd_SysPager = ::FindWindowEx(hWnd_TrayNotifyWnd,NULL,_T("SysPager"), NULL);        // WinXP
            // WinXP ������ SysPager ���� ����
            if(hWnd_SysPager) {
                hWnd_ToolbarWindow32 = ::FindWindowEx(hWnd_SysPager, NULL,_T("ToolbarWindow32"), NULL);
            // Win2000 �� ��쿡�� SysPager �� ���� TrayNotifyWnd -> ToolbarWindow32 �� �Ѿ��
            } else {
                hWnd_ToolbarWindow32 = ::FindWindowEx(hWnd_TrayNotifyWnd, NULL,_T("ToolbarWindow32"), NULL);
            }
        }
    }
    return hWnd_ToolbarWindow32;
} 

BOOL CUtilSys::RefreshTrayIcon()
{
    HANDLE     m_hProcess;
    LPVOID     m_lpData;

    TBBUTTON   tb;
    TRAYDATA   tray;
    DWORD      dwTrayPid;
    int        TrayCount;

    // Tray �� ������ �ڵ� ���
    HWND m_hTrayWnd = ::FindTrayToolbarWindow();
    if (m_hTrayWnd == NULL) {
        return FALSE;
    }

#if 0 /** ���۾���( http://dduk02.tistory.com/entry/Ʈ���̾�����-�ܻ�-���ֱ� ) */
    // Tray �� ������ ���ϰ�
    TrayCount = (int)::SendMessage(m_hTrayWnd, TB_BUTTONCOUNT, 0, 0);

    // Tray ������ �ڵ��� PID �� ���Ѵ�
    GetWindowThreadProcessId(m_hTrayWnd, &dwTrayPid);

    // �ش� Tray �� Process �� ��� �޸𸮸� �Ҵ��Ѵ�
    m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwTrayPid);

    if (!m_hProcess) {
        return FALSE;
    }

    // �ش� ���μ��� ���� �޸𸮸� �Ҵ�
    m_lpData = VirtualAllocEx(m_hProcess, NULL, sizeof (TBBUTTON), MEM_COMMIT, PAGE_READWRITE);
    if (!m_lpData) {
        return FALSE;
    }


    // Tray ��ŭ ������ 
    for(int i = 0; i < TrayCount; i++) {
        ::SendMessage(m_hTrayWnd, TB_GETBUTTON, i, (LPARAM)m_lpData);        

        // TBBUTTON �� ����ü�� TRAYDATA �� ������ ���
        ReadProcessMemory(m_hProcess, m_lpData, (LPVOID)&tb, sizeof (TBBUTTON), NULL);
        ReadProcessMemory(m_hProcess, (LPCVOID)tb.dwData, (LPVOID)&tray, sizeof (tray), NULL);

        // ���� Ʈ������ ���μ��� ��ȣ�� ��
        DWORD dwProcessId = 0;
        GetWindowThreadProcessId(tray.hwnd, &dwProcessId);

        // Process �� ���� ��� TrayIcon �� �����Ѵ�
        if (dwProcessId == 0) {
            NOTIFYICONDATA        icon;
            icon.cbSize           = sizeof(NOTIFYICONDATA);
            icon.hIcon            = tray.hIcon;
            icon.hWnd             = tray.hwnd;
            icon.uCallbackMessage = tray.uCallbackMessage;
            icon.uID              = tray.uID;

            Shell_NotifyIcon(NIM_DELETE, &icon);
        }
    }

    // ���� �޸� ������ ���μ��� �ڵ� �ݱ�
    VirtualFreeEx(m_hProcess, m_lpData, NULL, MEM_RELEASE);
    CloseHandle(m_hProcess);  
#else  /** �� ���� �� ( https://m.blog.naver.com/kil1rcn/30150374105 ) */
    if( m_hTrayWnd != NULL ) {
        CRect rect;
        ::GetClientRect( m_hTrayWnd, &rect );
        // get size of small icons
        DWORD dwWidth = (DWORD)GetSystemMetrics(SM_CXSMICON);   //  sm_cxsmicon);
        DWORD dwHeight = (DWORD)GetSystemMetrics(SM_CYSMICON);   //  sm_cysmicon);
        // initial y position of mouse - half of height of icon
        DWORD y = dwHeight >> 1;
        while( y < (DWORD)rect.bottom )  // while y < height of tray
        {
            DWORD x = dwHeight>>1;              // initial x position of mouse - half of width of icon
            while( x < (DWORD)rect.right ) // while x < width of tray
            {
                ::SendMessage( m_hTrayWnd, WM_MOUSEMOVE, 0, (y<<16)|x); // simulate moving mouse over an icon
                x = x + dwWidth; // add width of icon to x position
            }
            y = y + dwHeight; // add height of icon to y position
        }
    } 
#endif

    return TRUE;
}

CRect CUtilSys::GetMonitorResolution( DWORD dwMonitor )
{
    // 1. init the memebr rectangle
    m_rectMonitorResolution.SetRect(0,0,0,0);

    // 2. enumerate monitor 
    ::EnumDisplayMonitors(NULL, NULL, CUtilSys::MonitorEnumProc, dwMonitor );

    return m_rectMonitorResolution;
}

/**
 * [Note] Support for Only Primary Monitor 
 *   Width  - in millimeters, of the physical screen.
 *   Height - in millimeters, of the physical screen.
 */
CRect CUtilSys::GetMonitorPhysicalSize( DWORD dwMonitor )
{
    switch( dwMonitor ) {
        case PRIMARY_MONITOR : {
            HDC hScreenDC = GetDC(NULL);
            int hWidth  = ::GetDeviceCaps(hScreenDC,HORZSIZE);
            int hHeight = ::GetDeviceCaps(hScreenDC,VERTSIZE);

            return CRect( 0, 0, hWidth, hHeight );
        }
        break;
        default :
            ; 
    }

    return CRect( 0, 0, 0, 0 );
}

CRect CUtilSys::GetMonitorDpi( DWORD dwMonitor )
{
    UINT nDpiX = 0;
    UINT nDpiY = 0;
 
    HWND        hDesktopWnd = ::GetDesktopWindow();
    HMONITOR    hMonitor    = NULL;
    switch( dwMonitor ) {
        case PRIMARY_MONITOR : {
             hMonitor = ::MonitorFromWindow(hDesktopWnd, MONITOR_DEFAULTTOPRIMARY );
        }
        break;
        default : 
             hMonitor = ::MonitorFromWindow(hDesktopWnd, MONITOR_DEFAULTTONEAREST );
    }
    ::GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &nDpiX, &nDpiY);
    return CRect( 0, 0, nDpiX, nDpiY );
}

// 0.0 ~ 1.00(100%) ~ x.xx
double CUtilSys::GetLegibleRatioOfMonitor( DWORD dwMonitor )
{
#if 1 // resolution�� ���� ������ ������ ����ϰ�, ��ġ�ϴ� resolution�� ���� ��쿡�� "((130 / 1015)*(���ػ� ª�� �࡯-65))/100" ������ ����Ѵ�.
    CRect rectReslution = CUtilSys::GetMonitorResolution();
    int   nW       = rectReslution.Width(); 
    int   nH       = rectReslution.Height();

    if( nW == 1920   &&   nH == 1080 ) return 1.30;
    if( nW == 1600   &&   nH ==  900 ) return 1.07;
    if( nW == 1366   &&   nH ==  768 ) return 0.90;
    if( nW == 1680   &&   nH == 1050 ) return 1.26;
    if( nW == 1280   &&   nH == 1024 ) return 1.23;
    if( nW == 1440   &&   nH ==  900 ) return 1.07;
    if( nW == 1536   &&   nH ==  864 ) return 1.02;
    if( nW == 1829   &&   nH == 1029 ) return 1.23;
    if( nW == 1745   &&   nH ==  982 ) return 1.17;
    if( nW == 1920   &&   nH == 1200 ) return 1.45;
    if( nW == 1920   &&   nH == 1280 ) return 1.01;

#if 1 /** table�� ��쿡�� �����Ѵ�. ���Ƿ� ���� �������� �����̴�. */
    if( CUtilOS::IsTablet() == TRUE ) {
        if( nW == 1920   &&   nH == 1158 ) return 2.00;
    }
#endif

    if( nW < nH ) {
        return (((double)130/(double)1015)*(double)(nW-65))/(double)100;
    }

    return (((double)130/(double)1015)*(double)(nH-65))/(double)100;

#else 
    HMONITOR    hMonitor    = NULL;
    switch( dwMonitor ) {
        case PRIMARY_MONITOR : {
            CRect rectMonitorPhysical = CUtilSys::GetMonitorPhysicalSize(dwMonitor);  
            CRect rectMonitorDpi      = CUtilSys::GetMonitorDpi(dwMonitor);

            int nPhycalWidth  = rectMonitorPhysical.Width();
            int nPhycalHeight = rectMonitorPhysical.Height();
            int nDpiX         = rectMonitorDpi.Width();
            int nDpiY         = rectMonitorDpi.Height();

#if 1 // 20180103_jkim - Display Font Size���� ����� �ʿ�� ���� [[
            double dHorizontalLegibleRatio = (((double)CRITERION_MONITOR_WIDTH/(double)nPhycalWidth));
            double dVirticalLegibleRatio = (((double)CRITERION_MONITOR_HEIGHT/(double)nPhycalHeight));
#else
            double dHorizontalLegibleRatio = (((double)CRITERION_MONITOR_WIDTH/(double)nPhycalWidth)*((double)CRITERION_DPI_WIDTH/(double)nDpiX));
            double dVirticalLegibleRatio = (((double)CRITERION_MONITOR_HEIGHT/(double)nPhycalHeight)*((double)CRITERION_DPI_HEIGHT/(double)nDpiY));
#endif // ]]
            double dLegibleRatio = dHorizontalLegibleRatio;
            if( dHorizontalLegibleRatio < dVirticalLegibleRatio ) {
                dLegibleRatio = dHorizontalLegibleRatio;
            } 
            dLegibleRatio = 1.0 + ( dLegibleRatio - 1 )*0.50;
            if( dLegibleRatio < 1.0 ) {
                dLegibleRatio = 1.0;
            }

            return dLegibleRatio;
        }
        break;
        default : 
            ;
    }
#endif
    return (double)1.0;
}

void CUtilSys::OpenFileByEditor( CString strFilePath )
{
    CString     strEditor[4] = { "C:\\Program Files\\EditPlus 3\\editplus.exe",
                                 "C:\\Program Files\\Sublime Text 3\\sublime_text.exe",
                                 "C:\\Program Files (x86)\\Notepad++\\notepad++.exe",
                                 "notepad"
                                 };

    CFileFind finder;
    int nFind = 0;
    for ( nFind = 0; nFind < 2; nFind++) {
        if (finder.FindFile( strEditor[nFind] ) ) {
            break;
        }
    }

    ShellExecute(NULL, "open", strEditor[nFind], strFilePath, NULL, SW_SHOW);// ShellExecute(NULL, "open", strEditor, strItemPath, NULL, SW_SHOW);

}

void CUtilSys::Run( CString	strExePath,  // ���� ���� ���
					CString	strParam,    // Parameter
					CString	strExeDir,   // ���� ������ ���� ��ġ
					BOOL	bWait,       // �����Ҷ����� ��ٸ����� ����
                    BOOL    bSlient )    // slient mode
{
	//DEBUG_LOG( ">> CUtilShell::Run()" );
	//DEBUG_LOG( "++ strExePath=" + strExePath );
	//DEBUG_LOG( "++ strParam  =" + strParam );
	//DEBUG_LOG( "++ strExeDir =" + strExeDir );
	//DEBUG_LOG( "++ bWait     =" + CUtil2::Bool2Str( bWait ) );

	strExePath.Replace( '/', '\\' );

	// 0. �ʱ�ȭ
    SHELLEXECUTEINFO sei; 
    ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO)); 
    sei.cbSize			= sizeof(SHELLEXECUTEINFO); 
    sei.lpFile			= strExePath;
    sei.lpParameters	= strParam;
    sei.fMask			= SEE_MASK_NOCLOSEPROCESS; 
    sei.lpVerb			= "Open";
    sei.lpDirectory		= strExeDir;

    if( bSlient == TRUE ) {
        sei.nShow			= SW_HIDE; 
    } else {
        sei.nShow			= SW_SHOW;
    }
 
	// 1. ����
    ShellExecuteEx(&sei); 

	// 2. Wait
	if( bWait == TRUE ) {
		DWORD dwRet = WaitForSingleObject(sei.hProcess, INFINITE);

		switch( dwRet ) {
			case WAIT_FAILED : {
				//DEBUG_LOG( "++ WAIT_FAILED" );
			}
			break;
			case WAIT_ABANDONED : {
				//DEBUG_LOG( "++ WAIT_ABANDONED" );
			}
			break;
			case WAIT_OBJECT_0 : {
				//DEBUG_LOG( "++ WAIT_OBJECT_0" );
			}
			break;
			case WAIT_TIMEOUT : {
				//DEBUG_LOG( "++ WAIT_TIMEOUT" );
			}
			break;
			default : 
				//DEBUG_LOG( "++ default" );
				;
		}
	}
}

HWND CUtilSys::FindTopWindowByPartialName(CString strFindPartName)
{
    HWND hWndTemp = ::FindWindow(NULL, NULL); // �ֻ��� ������ �ڵ� ã��   
    char szTitle[1024] = { 0, }; 
    
    while (hWndTemp != NULL) {
        if (::GetParent(hWndTemp) == NULL){ // �ֻ��� �ڵ����� üũ, ��ư � �ڵ��� ���� �� �����Ƿ� �����ϱ� ����   
            memset( szTitle, 0, sizeof(szTitle)/sizeof(char));
            ::GetWindowText(hWndTemp, szTitle, sizeof(szTitle)/sizeof(char)-1);
            CString strTitle(szTitle);
            if (strTitle.IsEmpty() == false){
                if (strTitle.Find(strFindPartName) > -1){
                    return hWndTemp;
                }
            }
        }
        hWndTemp = ::GetWindow(hWndTemp, GW_HWNDNEXT); // ���� ������ �ڵ� ã��   
    }
    return NULL;
}

// http://egloos.zum.com/katalog/v/4410108
HWND CUtilSys::FindAllWindowByPartialName(CString strFindPartName)
{
    return CUtilSys::FindAllWindowByPartialName( strFindPartName, ::GetDesktopWindow(), 0);
}

HWND CUtilSys::FindAllWindowByPartialName( CString strFindPartName, HWND hWndParent, LPARAM lParam )
{
    static char szTitle[1024] = { 0, }; 

    // check 
    memset( szTitle, 0, sizeof(szTitle)/sizeof(char));
    ::GetWindowText(hWndParent, szTitle, sizeof(szTitle)/sizeof(char)-1);
    CString strTitle(szTitle);
    if (strTitle.IsEmpty() == false){
        if (strTitle.Find(strFindPartName) > -1){
            return hWndParent;
        }
    }

    // iterator
    for(HWND hWndChild = ::GetWindow(hWndParent, GW_CHILD);
        hWndChild != NULL;
        hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT)) {
        // recursive
        HWND hWndBingo = CUtilSys::FindAllWindowByPartialName( strFindPartName, hWndChild, lParam+4);
        if( hWndBingo != NULL ) {
            return hWndBingo;
        }
    }

    return NULL;
}

CString CUtilSys::GetWindowTitleByPartialName(CString strFindPartName)
{
    HWND hWnd = CUtilSys::FindAllWindowByPartialName(strFindPartName);
    if( hWnd != NULL ) {
        char szTitle[1024] = { 0, }; 
        ::GetWindowText(hWnd, szTitle, sizeof(szTitle)/sizeof(char)-1);
        return CString(szTitle);
    }

    return _T("");
}

/* 
 * > ������ ������ Window�� ���� process�� ��� �ִ� ���.
 * > bFindPartName : 
 *   - TRUE  : �Է��� strWindowTitle ������ �� ���α׷�(������) Ÿ��Ʋ �̸� �˻� �� �ߺ� �˻�,  
 *   - FALSE : �Է��� strWindowTitle �� ��ġ �ϴ� ���α׷�(������) �ߺ��˻�
 */
BOOL CUtilSys::IsRunZombi(CString strWindowTitle, CString strProgramName,  BOOL bFindPartialName)
{
    HWND hWnd = NULL;
   
    // 1. ã�� ������ Ÿ��Ʋ �˻�
    if (bFindPartialName == TRUE ) { 
        hWnd = CUtilSys::FindAllWindowByPartialName(strWindowTitle);
    } else {   
        hWnd = ::FindWindow(NULL, strWindowTitle);
    }

    // 2. process id ã��  
    ULONG ulProcessID = CUtilSys::GetFindProcessID(strProgramName);
    
    // 3. �����찡 ����, process id�� �ִ� ���� �����.
    if( hWnd == NULL && ulProcessID != NULL ) {    
        ULONG ulSelfProcessID = GetCurrentProcessId();
        // �ڽ��� �ƴϿ��� �Ѵ�.
        if( ulSelfProcessID != ulProcessID ) {
	        return TRUE;
        }
    }

    return FALSE;
}

/*
 * 1. ���� ������ ���̰�
 * 2. ���� ����, 
 *    2.1. �������� ���μ����� ������, Active ��Ű��
 *    2.2. �������� ���μ����� ������, Skip �Ѵ�( ��, �� Process�� Active�ȴ� ). 
 * 3. return value
 *    - TRUE  : ���� App�� Active�� ������
 *    - FALSE : ���� App�� Active�� ��Ű�� �ʾ���
 */
BOOL CUtilSys::PreventDuplicateExecution(CString strWindowTitle, CString strProgramName,  BOOL bFindPartialName)
{
    // 1. ����� ���δ�. 
    BOOL bRunZombi = IsRunZombi(strWindowTitle, strProgramName, bFindPartialName );
    if( bRunZombi == TRUE ) {
        CUtilSys::KillProcessByName(strProgramName);        
    }

    // 2. ã�� ������ Ÿ��Ʋ �˻�
    CString strFullTitle = strWindowTitle;
    //  - bFindPartName : 
    //      TRUE  : �Է��� strWindowTitle ������ �� ���α׷�(������) Ÿ��Ʋ �̸� �˻� �� �ߺ� �˻�,  
    //      FALSE : �Է��� strWindowTitle �� ��ġ �ϴ� ���α׷�(������) �ߺ��˻�
    if (bFindPartialName == TRUE ) { 
        strFullTitle = CUtilSys::GetWindowTitleByPartialName(strWindowTitle);
        if( strFullTitle.IsEmpty() == TRUE ) {
            strFullTitle = strWindowTitle;
        }
    }

    // 3. ���μ����� ����ְ� �����쵵 ���� ! �׷��ٸ� ã�Ƽ� ����ش�
    HWND hWndFind = ::FindWindow(NULL, strFullTitle.GetBuffer());
    if (hWndFind != NULL ) {
        // ���α׷�(������) �����
        CUtilSys::ActivateWindow( hWndFind );
        return TRUE; // ���� App�� Active�� ������
    }

    return FALSE; // ���� App�� Active�� ��Ű�� �ʾ���
}

void CUtilSys::ActivateWindow( HWND hWnd, BOOL bTop )
{
    if( hWnd != NULL ) {

        // top & active
        if( bTop == TRUE ) {
	        ::SetForegroundWindow(hWnd);
            ::SetActiveWindow(hWnd);
        }
        ::ShowWindow(hWnd, SW_SHOW);  
        
        // maximize or normal 
	    WINDOWPLACEMENT wp;
        ::GetWindowPlacement( hWnd, &wp );
        UINT nCurShow = wp.showCmd;
        if( nCurShow == SW_SHOWMAXIMIZED ) {
            ::ShowWindow( hWnd, SW_SHOWMAXIMIZED);
        } else {
            ::ShowWindow( hWnd, SW_SHOWNORMAL );
        }
    }
}

ULONG CUtilSys::GetFindProcessID(CString strProgramName)
{
    HANDLE hSnapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof(pEntry);
    BOOL bResult = Process32First(hSnapShot, &pEntry);
     
    if (!bResult){
        CloseHandle(hSnapShot);
        return NULL;
    }

    do{
        if (strProgramName.CompareNoCase(pEntry.szExeFile) == 0) {
            // �ڽ��� �ƴϾ�� �Ѵ�. 
            if( ::GetCurrentThreadId() != pEntry.th32ProcessID ) {
                return pEntry.th32ProcessID;
            }
        }
    } while (Process32Next(hSnapShot, &pEntry));

    return NULL;
}
/** 
 * process�� ���δ�.
 */
void CUtilSys::KillProcessByName( CString strProgramName, BOOL bForcibly )
{
    ULONG ulProcessID = CUtilSys::GetFindProcessID(strProgramName);
    if( ulProcessID != NULL ) { 
        // process�� ������ ���δ�. 
        if( bForcibly == TRUE ) {
            CString strExePath  = "TASKKILL";
            CString	strParam    = "";   
            CString	strExeDir   = "";
            BOOL	bWait       = FALSE;

            strParam.Format( "/F /PID %d", ulProcessID );

            CUtilSys::Run(  strExePath,  // ���� ���� ��� 
                            strParam,    // Parameter 
                            strExeDir,   // ���� ������ ���� ��ġ
                            bWait  );    // �����Ҷ����� ��ٸ����� ����
        // process���� APP_EXIT�� ������ �׾� �޶�� ��û�Ѵ�.
        } else {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)ulProcessID );
            if (hProcess != NULL) {
                TerminateProcess(hProcess, 9);
                CloseHandle(hProcess);
            }
        }
    }
}


#if 0 // Not Use : ������ Ȯ�ε��� �ʾ���
/** 
 * https://stackoverflow.com/questions/7956519/how-to-kill-processes-by-name-win32-api 
 */
void CUtilSys::KillProcessByName2( CString strName )
{
    const int maxProcIds = 1024;
    DWORD procList[maxProcIds];
    DWORD procCount;
    char processName[MAX_PATH];

    // get the process by name
    if (!::EnumProcesses(procList, sizeof(procList), &procCount))
        return;

    // convert from bytes to processes
    procCount = procCount / sizeof(DWORD);

    // loop through all processes
    for (DWORD procIdx=0; procIdx<procCount; procIdx++) {
        // get a handle to the process
        HANDLE procHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procList[procIdx]);
        // get the process name
        ::GetProcessImageFileName(procHandle, processName, sizeof(processName));
        // terminate all pocesses that contain the name
        if ( strName.CompareNoCase(processName) == 0 ) {
            TerminateProcess(procHandle, 0);
        }
        CloseHandle(procHandle);    
    }
}
#endif

/*
 * http://www.programmersheaven.com/discussion/197697/how-can-i-verify-that-an-exe-file-is-really-a-valid-executable
 * 
 */
BOOL CUtilSys::IsExecutableFile( CString strPath )
{
    DWORD dwBinaryType;
    int nResult = GetBinaryType( strPath, &dwBinaryType);
    if( nResult != 0 ) {
        switch( dwBinaryType ) {
            case SCS_32BIT_BINARY : return TRUE;  // 32-bit Windows-based application.
            case SCS_64BIT_BINARY : return TRUE;  // 64-bit Windows-based application.
            case SCS_DOS_BINARY   : return FALSE; // MS-DOS based application.
            case SCS_OS216_BINARY : return FALSE; // 16-bit OS/2-based application.
            case SCS_PIF_BINARY   : return FALSE; // PIF file that executes an MS-DOS ? based application.
            case SCS_POSIX_BINARY : return FALSE; // POSIX based application.
            case SCS_WOW_BINARY   : return FALSE; // 16-bit Windows-based application
        }
    } 

    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// private function
////////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK CUtilSys::MonitorEnumProc(   HMONITOR    hMonitor,       // handle to display monitor
                                           HDC         hdc1,           // handle to monitor DC
                                           LPRECT      lprcMonitor,    // monitor intersection rectangle
                                           LPARAM      dwMonitor )     // MONITORINFOF_PRIMARY or 
{
    RECT rc = *lprcMonitor;

    MONITORINFOEX mInfo;
    mInfo.cbSize = sizeof(mInfo);
    ::GetMonitorInfo(hMonitor, &mInfo);

    switch( dwMonitor ) {
        case PRIMARY_MONITOR : {
            if( MONITORINFOF_PRIMARY == mInfo.dwFlags ) {
                m_rectMonitorResolution = mInfo.rcWork;
                // return FALSE to stop the enumeration
                return FALSE;
            }
        }
        break;
        case SECONDARY_MONITOR : {
            if( MONITORINFOF_PRIMARY != mInfo.dwFlags ) {
                m_rectMonitorResolution = mInfo.rcWork;
                // return FALSE to stop the enumeration
                return FALSE;
            }
        }
        break;
    }

    // if we get here, the current monitor isn't a secondary monitor
    return TRUE;
}

//��ġ ������ ���� �Լ� 
void CUtilSys::RunDeviceManager()
{
	OSVERSIONINFO verInfo;
	verInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO); // ��ǻ���� os ������ �޾ƿ´�.

	if (::GetVersionEx(&verInfo))
	{
		if (verInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			// NT�迭
			ShellExecute(NULL, NULL, "devmgmt.msc", NULL, NULL, SW_SHOWNORMAL);

			return;
		}
	}
}

void CUtilSys::DeleteFilesByDay  ( CString strDirPath, int nDays )
{
    DeleteFilesByTime ( DEL_STANDARD_TYPE_BY_DAYS , strDirPath, nDays);
}   
void CUtilSys::DeleteFilesByMonth( CString strDirPath, int nMonth)
{
    DeleteFilesByTime ( DEL_STANDARD_TYPE_BY_MONTH , strDirPath, nMonth * 30);
}

// �ð� ���� Ȥ�� �ٸ� ������ ����� �� �� �����Ƿ�, Day �� �и�
void CUtilSys::DeleteFilesByTime ( int nStandardType, CString strDirPath, int nTimeValue)
{

    CFileFind fFind;
    CString strDirectory = strDirPath;

    CStringArray srFileArray;

    if(strDirectory.Right(1) != '\\'){
        strDirectory += '\\';
    }
    strDirectory += "*.*";

    bool  bFind = fFind.FindFile(strDirectory);
    if(  bFind ){
        CString strFileName;
        BOOL bFlag = TRUE;
        while( bFlag == TRUE){
            bFlag = fFind.FindNextFile();
            strFileName = fFind.GetFilePath();
            if( fFind.IsDots()){
                continue;
            }else if( fFind.IsDirectory()){
                DeleteFilesByTime ( nStandardType,fFind.GetFilePath(), nTimeValue);
            }else{
                HANDLE     hFileHandle = CreateFile( fFind.GetFileName(),GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
                SYSTEMTIME stTime;
                CTime      tLastWriteTime;
                
                ::GetLocalTime(&stTime);
                CTime      tToday(stTime);
                fFind.GetLastWriteTime(tLastWriteTime);
                
                CTimeSpan tSpan = tToday - tLastWriteTime;
                if(tSpan.GetDays() >= nTimeValue){
                    ::DeleteFile(strFileName);
                }
                
            }
        }
    }

}   
