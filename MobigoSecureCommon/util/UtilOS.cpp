////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "utilos.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// static member
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CUtilOS::CUtilOS(void)
{
    
}

CUtilOS::~CUtilOS(void)
{
}

////////////////////////////////////////////////////////////////////////////////
// public 
////////////////////////////////////////////////////////////////////////////////

/*
 * https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms724439(v=vs.85).aspx
 * https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms724832(v=vs.85).aspx
 * http://0x616b616d61.tistory.com/entry/운영체제-버전-확인하기Get-OS-Version
 * https://stackoverflow.com/questions/32193855/c-check-if-windows-10 
 */

CString CUtilOS::GetOSVersion()
{
    DWORD dwVersion = 0; 
    DWORD dwMajorVersion = 0;
    DWORD dwMinorVersion = 0; 
    DWORD dwBuild = 0;

    dwVersion = ::GetVersion();
 
    // Get the Windows version.
    dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
    dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

    // Get the build number.
    if (dwVersion < 0x80000000) {              
        dwBuild = (DWORD)(HIWORD(dwVersion));
    }

    CString strOSVersion = "";
    if( 6 <= dwMajorVersion ) {
        int nWindow10Version = GetMajorVersionNumberFromRegistry();
        if( 10 <= nWindow10Version ) {
            strOSVersion.Format( "%d.%d ( build=%d )", 
                        nWindow10Version,
                        0,
                        dwBuild);
        } 
    } 

    if( strOSVersion.IsEmpty() == TRUE ) {
        strOSVersion.Format( "%d.%d ( build=%d )", 
                    dwMajorVersion,
                    dwMinorVersion,
                    dwBuild);
    }
    return strOSVersion;
}
 
////////////////////////////////////////////////////////////////////////////////
// 32 bit / 64 bit
////////////////////////////////////////////////////////////////////////////////

BOOL CUtilOS::Is64BitOS()
{
    SYSTEM_INFO   sInfo;
    GetNativeSystemInfo(&sInfo);
    if (sInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
        return TRUE;
    }

    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// Tablet
////////////////////////////////////////////////////////////////////////////////

BOOL CUtilOS::IsTablet()
{
    int nTablet = ::GetSystemMetrics(SM_TABLETPC);
    if( nTablet != 0 ) {
        return TRUE;
    }
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// https://stackoverflow.com/questions/9817160/getversionex-under-windows-8
////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <iostream>
using namespace std;
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

#pragma comment(lib, "User32.lib")

#define BUFSIZE 256

typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);

CString CUtilOS::GetOSName()
{
   TCHAR szOS[256];
   OSVERSIONINFOEX osvi;
   SYSTEM_INFO si;
   PGNSI pGNSI;
   PGPI pGPI;
   BOOL bOsVersionInfoEx;
   DWORD dwType;

   ZeroMemory(szOS, sizeof(szOS)/sizeof(TCHAR));
   ZeroMemory(&si, sizeof(SYSTEM_INFO));
   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
   bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*) &osvi);

   if( ! bOsVersionInfoEx ) return "Unknown OS Name";;

   // Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.

   pGNSI = (PGNSI) GetProcAddress(
      GetModuleHandle(TEXT("kernel32.dll")), 
      "GetNativeSystemInfo");
   if(NULL != pGNSI)
      pGNSI(&si);
   else GetSystemInfo(&si);

   if ( VER_PLATFORM_WIN32_NT==osvi.dwPlatformId && 
        osvi.dwMajorVersion > 4 )
   {
      StringCchCopy(szOS, BUFSIZE, TEXT("Microsoft "));

      if( 6 <= osvi.dwMajorVersion && 10 <= GetMajorVersionNumberFromRegistry() ) {
         StringCchCat(szOS, BUFSIZE, TEXT("Windows 10 "));
      } else if ( osvi.dwMajorVersion == 6 ) {
         if( osvi.dwMinorVersion == 0 )
         {
            if( osvi.wProductType == VER_NT_WORKSTATION )
                StringCchCat(szOS, BUFSIZE, TEXT("Windows Vista "));
            else StringCchCat(szOS, BUFSIZE, TEXT("Windows Server 2008 " ));
         }

         if ( osvi.dwMinorVersion == 1 || osvi.dwMinorVersion == 2 )
         {
            if ( osvi.wProductType == VER_NT_WORKSTATION && osvi.dwMinorVersion == 1)
                StringCchCat(szOS, BUFSIZE, TEXT("Windows 7 "));
            else
            if ( osvi.wProductType == VER_NT_WORKSTATION && osvi.dwMinorVersion == 2)
                StringCchCat(szOS, BUFSIZE, TEXT("Windows 8 "));
            else 
                StringCchCat(szOS, BUFSIZE, TEXT("Windows Server 2008 R2 " ));
         }

         pGPI = (PGPI) GetProcAddress(
            GetModuleHandle(TEXT("kernel32.dll")), 
            "GetProductInfo");

         pGPI( osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);

         switch( dwType )
         {
            case PRODUCT_ULTIMATE:
               StringCchCat(szOS, BUFSIZE, TEXT("Ultimate Edition" ));
               break;
            case PRODUCT_PROFESSIONAL:
               StringCchCat(szOS, BUFSIZE, TEXT("Professional" ));
               break;
            case PRODUCT_HOME_PREMIUM:
               StringCchCat(szOS, BUFSIZE, TEXT("Home Premium Edition" ));
               break;
            case PRODUCT_HOME_BASIC:
               StringCchCat(szOS, BUFSIZE, TEXT("Home Basic Edition" ));
               break;
            case PRODUCT_ENTERPRISE:
               StringCchCat(szOS, BUFSIZE, TEXT("Enterprise Edition" ));
               break;
            case PRODUCT_BUSINESS:
               StringCchCat(szOS, BUFSIZE, TEXT("Business Edition" ));
               break;
            case PRODUCT_STARTER:
               StringCchCat(szOS, BUFSIZE, TEXT("Starter Edition" ));
               break;
            case PRODUCT_CLUSTER_SERVER:
               StringCchCat(szOS, BUFSIZE, TEXT("Cluster Server Edition" ));
               break;
            case PRODUCT_DATACENTER_SERVER:
               StringCchCat(szOS, BUFSIZE, TEXT("Datacenter Edition" ));
               break;
            case PRODUCT_DATACENTER_SERVER_CORE:
               StringCchCat(szOS, BUFSIZE, TEXT("Datacenter Edition (core installation)" ));
               break;
            case PRODUCT_ENTERPRISE_SERVER:
               StringCchCat(szOS, BUFSIZE, TEXT("Enterprise Edition" ));
               break;
            case PRODUCT_ENTERPRISE_SERVER_CORE:
               StringCchCat(szOS, BUFSIZE, TEXT("Enterprise Edition (core installation)" ));
               break;
            case PRODUCT_ENTERPRISE_SERVER_IA64:
               StringCchCat(szOS, BUFSIZE, TEXT("Enterprise Edition for Itanium-based Systems" ));
               break;
            case PRODUCT_SMALLBUSINESS_SERVER:
               StringCchCat(szOS, BUFSIZE, TEXT("Small Business Server" ));
               break;
            case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
               StringCchCat(szOS, BUFSIZE, TEXT("Small Business Server Premium Edition" ));
               break;
            case PRODUCT_STANDARD_SERVER:
               StringCchCat(szOS, BUFSIZE, TEXT("Standard Edition" ));
               break;
            case PRODUCT_STANDARD_SERVER_CORE:
               StringCchCat(szOS, BUFSIZE, TEXT("Standard Edition (core installation)" ));
               break;
            case PRODUCT_WEB_SERVER:
               StringCchCat(szOS, BUFSIZE, TEXT("Web Server Edition" ));
               break;
         }
      }

      if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
      {
         if( GetSystemMetrics(SM_SERVERR2) )
            StringCchCat(szOS, BUFSIZE, TEXT( "Windows Server 2003 R2, "));
         else if ( osvi.wSuiteMask & VER_SUITE_STORAGE_SERVER )
            StringCchCat(szOS, BUFSIZE, TEXT( "Windows Storage Server 2003"));
         else if ( osvi.wSuiteMask & VER_SUITE_WH_SERVER )
            StringCchCat(szOS, BUFSIZE, TEXT( "Windows Home Server"));
         else if( osvi.wProductType == VER_NT_WORKSTATION &&
                  si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
         {
            StringCchCat(szOS, BUFSIZE, TEXT( "Windows XP Professional x64 Edition"));
         }
         else StringCchCat(szOS, BUFSIZE, TEXT("Windows Server 2003, "));

         // Test for the server type.
         if ( osvi.wProductType != VER_NT_WORKSTATION )
         {
            if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_IA64 )
            {
                if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                   StringCchCat(szOS, BUFSIZE, TEXT( "Datacenter Edition for Itanium-based Systems" ));
                else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                   StringCchCat(szOS, BUFSIZE, TEXT( "Enterprise Edition for Itanium-based Systems" ));
            }

            else if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
            {
                if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                   StringCchCat(szOS, BUFSIZE, TEXT( "Datacenter x64 Edition" ));
                else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                   StringCchCat(szOS, BUFSIZE, TEXT( "Enterprise x64 Edition" ));
                else StringCchCat(szOS, BUFSIZE, TEXT( "Standard x64 Edition" ));
            }

            else
            {
                if ( osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER )
                   StringCchCat(szOS, BUFSIZE, TEXT( "Compute Cluster Edition" ));
                else if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
                   StringCchCat(szOS, BUFSIZE, TEXT( "Datacenter Edition" ));
                else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
                   StringCchCat(szOS, BUFSIZE, TEXT( "Enterprise Edition" ));
                else if ( osvi.wSuiteMask & VER_SUITE_BLADE )
                   StringCchCat(szOS, BUFSIZE, TEXT( "Web Edition" ));
                else StringCchCat(szOS, BUFSIZE, TEXT( "Standard Edition" ));
            }
         }
      }

      if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
      {
         StringCchCat(szOS, BUFSIZE, TEXT("Windows XP "));
         if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
            StringCchCat(szOS, BUFSIZE, TEXT( "Home Edition" ));
         else StringCchCat(szOS, BUFSIZE, TEXT( "Professional" ));
      }

      if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
      {
         StringCchCat(szOS, BUFSIZE, TEXT("Windows 2000 "));

         if ( osvi.wProductType == VER_NT_WORKSTATION )
         {
            StringCchCat(szOS, BUFSIZE, TEXT( "Professional" ));
         }
         else 
         {
            if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
               StringCchCat(szOS, BUFSIZE, TEXT( "Datacenter Server" ));
            else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
               StringCchCat(szOS, BUFSIZE, TEXT( "Advanced Server" ));
            else StringCchCat(szOS, BUFSIZE, TEXT( "Server" ));
         }
      }

       // Include service pack (if any) and build number.

      if( _tcslen(osvi.szCSDVersion) > 0 )
      {
          StringCchCat(szOS, BUFSIZE, TEXT(" ") );
          StringCchCat(szOS, BUFSIZE, osvi.szCSDVersion);
      }

      TCHAR buf[80];
      ZeroMemory(buf, sizeof(buf)/sizeof(TCHAR));
      StringCchPrintf( buf, 80, TEXT(" (build %d)"), osvi.dwBuildNumber);
      StringCchCat(szOS, BUFSIZE, buf);

      if ( osvi.dwMajorVersion >= 6 )
      {
         if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
            StringCchCat(szOS, BUFSIZE, TEXT( ", 64-bit" ));
         else if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_INTEL )
            StringCchCat(szOS, BUFSIZE, TEXT(", 32-bit"));
      }
      return CString(szOS); 
   } else {  
      return "Unknown OS Name";
   }
}

////////////////////////////////////////////////////////////////////////////////
// private
////////////////////////////////////////////////////////////////////////////////

// http://bbangdeveloper.tistory.com/entry/Windows-OS-버전-가져오는-소스
int CUtilOS::GetMajorVersionNumberFromRegistry()
{
    LONG lResult;
    HKEY hKey;
    DWORD dwType;
    DWORD dwBytes;
    int buffer = 0;
 
    // open Regstry Key
    lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
        _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"),
        0, KEY_QUERY_VALUE, &hKey);
    if(lResult != ERROR_SUCCESS)
    {
        return -1;
    }
 
    // Read Regstry Key
    lResult=RegQueryValueEx(hKey, _T("CurrentMajorVersionNumber"), 0, &dwType, (LPBYTE)&buffer, &dwBytes);
    if(lResult == ERROR_SUCCESS)
    {
        return buffer;
    }
 
    RegCloseKey(hKey);
    return -1;
}