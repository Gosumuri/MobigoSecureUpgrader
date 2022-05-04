#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include <Winbase.h> 

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

#define PRIMARY_MONITOR             MONITORINFOF_PRIMARY
#define SECONDARY_MONITOR           0x00000002    

// 파일 삭제를 위한 기준
#define DEL_STANDARD_TYPE_BY_DAYS         1
#define DEL_STANDARD_TYPE_BY_MONTH        2

////////////////////////////////////////////////////////////////////////////////
// static 
////////////////////////////////////////////////////////////////////////////////

#ifndef GET_MODULE_NAME
#define GET_MODULE_NAME
static CString GetModuleName()
{
    CString strModulePath = _T("");
    CHAR path[MAX_PATH];
    HMODULE hm;
    if (::GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPWSTR)&::GetModuleName, &hm)) {
        ::GetModuleFileName(hm, path, sizeof(path));
        strModulePath = CString(path);
        strModulePath.Replace('/', '\\');

        int nFirstIndex = strModulePath.ReverseFind('\\');
        if (nFirstIndex == -1) {
            return strModulePath;
        }

        return strModulePath.Mid(nFirstIndex + 1);
    }
    return strModulePath;
}
#endif

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////

class AFX_EXT_CLASS CUtilSys
{
public:
	CUtilSys(void);
	~CUtilSys(void);

////////////////////////////////////////////////////////////////////////////////
// member data
////////////////////////////////////////////////////////////////////////////////

private :
    static CRect            m_rectMonitorResolution;

////////////////////////////////////////////////////////////////////////////////
// member function
////////////////////////////////////////////////////////////////////////////////

public :	
    static BOOL             IsNetworkConnection();
    static BOOL             IsWirelessNetworkConnection();
    static HRESULT          CreateShortcut( /*in*/  LPCTSTR lpszFileName, 
                                            /*in*/  LPCTSTR lpszDesc, 
                                            /*in*/  LPCTSTR lpszShortcutPath);
    static HRESULT          ResolveShortcut(/*in*/  LPCTSTR lpszShortcutPath,
                                            /*out*/ LPTSTR  lpszFilePath);
    static BOOL             CreateDesktopShortcut(  LPTSTR      pszFilePath, 
                                                    LPTSTR      pszTitle );
    static long             GetDiskTotalSpace(CString strDisk, BOOL bGiga = TRUE );
    static long             GetDiskUsedSpace( CString strDisk, BOOL bGiga = TRUE );
    static long             GetDiskFreeSpace( CString strDisk, BOOL bGiga = TRUE );
    static CString          GetIEVersion();
    static CString          GetComputerName();
    static CString          GetVolumeSerial();
    static CString          GetGateWayIPAddress();
    static CString          GetMACAddress();
    static CString          GetIPv4IPAddress();
    static CString          GetDNSAddress();
    static CString          GetMACAddress( CString address );
    static HWND             FindTopWindowByPartialName( CString strPartialName );      // 입력한 strPartName 가 포함된 Top 윈도우 찾기 
    static HWND             FindAllWindowByPartialName( CString strPartialName );      // 입력한 strPartName 가 포함된 모든 윈도우 찾기 
    static HWND             FindAllWindowByPartialName( CString strPartialName, HWND hwnd, LPARAM lParam);
    static CString          GetWindowTitleByPartialName( CString strPartialName );  // 입력한 strPartName 가 포함된 프로그램(윈도우) 타이틀 찾기 
    static BOOL             RefreshTrayIcon();
    static CRect            GetMonitorResolution( DWORD dwMonitor = PRIMARY_MONITOR );
    static CRect            GetMonitorPhysicalSize( DWORD dwMonitor = PRIMARY_MONITOR );
    static CRect            GetMonitorDpi( DWORD dwMonitor = PRIMARY_MONITOR );
    static double           GetLegibleRatioOfMonitor( DWORD dwMonitor = PRIMARY_MONITOR );
    static void             OpenFileByEditor( CString strFilePath );   
    static void             DeleteFilesByDay  ( CString strDirPath, int nDays );   
    static void             DeleteFilesByMonth( CString strDirPath, int nMonth);   
    static BOOL             IsRun( CString strWindowTitle, CString strProgramName, BOOL bFindPartialName);
    static BOOL             IsRunZombi( CString strWindowTitle, CString strProgramName, BOOL bFindPartialName);
	static void	            Run(	CString	strExePath,         // 실행 파일 경로
									CString	strParam,           // Parameter
									CString	strExeDir,          // 실행 파일의 실행 위치
									BOOL	bWait   = FALSE,    // 종료할때까지 기다릴지의 여부
									BOOL	bSilent = FALSE );  // Slient Mode
    // 중복 검사 후 중복될 경우 열려있는 프로그램 보여줌
    static BOOL             PreventDuplicateExecution( CString strWindowTitle,              //중복검사할 프로그램(윈도우) 타이틀 이름
                                                       CString strProgramName = CString(),  // 프로세스가 존재하는 지 검색할 프로그램 이름
                                                       BOOL    bFindPartialName = FALSE);   // TRUE  : 입력한 strWindowTitle 내용이 들어간 프로그램(윈도우) 타이틀 이름 검색 후 중복 검사,  
                                                                                            // FALSE : 입력한 strWindowTitle 과 일치 하는 프로그램(윈도우) 중복검사 
    static void             ActivateWindow( HWND hWnd, BOOL bTop = TRUE );
    static void             KillProcessByName(          CString strName, BOOL bForcibly = FALSE );
#if 0 // 동작이 확인 되지 않음
    static void             KillProcessByName2( CString strName );
#endif
    static void             RunDeviceManager( );
    static ULONG            GetFindProcessID(   CString strName );
    static BOOL             IsExecutableFile(   CString strPath );
    
private :
    static BOOL CALLBACK    MonitorEnumProc(    HMONITOR    hMonitor,       // handle to display monitor
                                                HDC         hdc1,           // handle to monitor DC
                                                LPRECT      lprcMonitor,    // monitor intersection rectangle
                                                LPARAM      data);          // data
    static void             DeleteFilesByTime ( int nStandardType , CString strDirPath, int nTimeValue);   
};
