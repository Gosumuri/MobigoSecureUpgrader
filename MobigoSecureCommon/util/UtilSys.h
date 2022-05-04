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

// ���� ������ ���� ����
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
    static HWND             FindTopWindowByPartialName( CString strPartialName );      // �Է��� strPartName �� ���Ե� Top ������ ã�� 
    static HWND             FindAllWindowByPartialName( CString strPartialName );      // �Է��� strPartName �� ���Ե� ��� ������ ã�� 
    static HWND             FindAllWindowByPartialName( CString strPartialName, HWND hwnd, LPARAM lParam);
    static CString          GetWindowTitleByPartialName( CString strPartialName );  // �Է��� strPartName �� ���Ե� ���α׷�(������) Ÿ��Ʋ ã�� 
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
	static void	            Run(	CString	strExePath,         // ���� ���� ���
									CString	strParam,           // Parameter
									CString	strExeDir,          // ���� ������ ���� ��ġ
									BOOL	bWait   = FALSE,    // �����Ҷ����� ��ٸ����� ����
									BOOL	bSilent = FALSE );  // Slient Mode
    // �ߺ� �˻� �� �ߺ��� ��� �����ִ� ���α׷� ������
    static BOOL             PreventDuplicateExecution( CString strWindowTitle,              //�ߺ��˻��� ���α׷�(������) Ÿ��Ʋ �̸�
                                                       CString strProgramName = CString(),  // ���μ����� �����ϴ� �� �˻��� ���α׷� �̸�
                                                       BOOL    bFindPartialName = FALSE);   // TRUE  : �Է��� strWindowTitle ������ �� ���α׷�(������) Ÿ��Ʋ �̸� �˻� �� �ߺ� �˻�,  
                                                                                            // FALSE : �Է��� strWindowTitle �� ��ġ �ϴ� ���α׷�(������) �ߺ��˻� 
    static void             ActivateWindow( HWND hWnd, BOOL bTop = TRUE );
    static void             KillProcessByName(          CString strName, BOOL bForcibly = FALSE );
#if 0 // ������ Ȯ�� ���� ����
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
