#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


// http retry count
#define     HTTP_RETRY_COUNT                                    3

// Buffer Size
#define		MAX_BUF							                    256

// minimum
#define     MINIMUM_DISK_SPACE                                  1024                                                // Mega Byte

// name
#define     SMART_MOBIGO_ACTIVEX_NAME                           _T("Smart Mobigo ActiveX")

#define     MOBIGO_SECURE_APP_NAME                              _T("Mobigo Secure Application")
#define     MOBIGO_SECURE_APP_MAIN_FRAME_NAME                   _T("Mobigo Secure MainFrame")                       // release 모드에서는 hide된다.
#define     MOBIGO_SECURE_APP_DATA_PATH                         _T("mobigo_secure_app")
#define     MOBIGO_SECURE_APP_EXE                               _T("MobigoSecureApp.exe")
#define     MOBIGO_SECURE_APP_DEBUG_NAME                        _T("Mobigo Secure App Debugger")

#define     MOBIGO_SECURE_UPGRADER_NAME                         _T("Mobigo Secure Upgrader")
#define     MOBIGO_SECURE_UPGRADER_EXE                          _T("MobigoSecureUpgrader.exe")
#define     MOBIGO_SECURE_UPGRADER_DATA_PATH                    _T("mobigo_secure_up")
#define     MOBIGO_SECURE_UPGRADER_DEBUG_NAME                   _T("Mobigo Secure Upgrader Debugger")

// path
#define     MOBIGO_DEFAULT_ROOT_PATH                            _T("C:\\mobigo")
#define     MOBIGO_ROOT_PATH                                    ((LPCTSTR)CUtilEnv::GetMobigoRootPath())                // _T("c:\\mobigo")
#define     MOBIGO_TEMP_PATH                                    ((LPCTSTR)CUtilEnv::GetMobigoTempPath())                // _T("c:\\mobogo\\upgrader\\temp" )
#define     MOBIGO_SECURE_APP_EXE_PATH                          ((LPCTSTR)CUtilEnv::GetMobigoSecureAppExePath())        // _T("C:\\mobigo\\MobigoSecureApp.exe" )
#define     MOBIGO_SECURE_KILLER_EXE_PATH                       ((LPCTSTR)CUtilEnv::GetMobigoSecureKillerExePath())     // _T("C:\\mobigo\\MobigoSecureKiller.exe" )
#define     MOBIGO_SECURE_NOTHING_EXE_PATH                      ((LPCTSTR)CUtilEnv::GetMobigoSecureNothingExePath())    // _T("C:\\mobigo\\MobigoSecureNothing.exe" )
#define     MOBIGO_SECURE_REBOOTER_EXE_PATH                     ((LPCTSTR)CUtilEnv::GetMobigoSecureRebooterExePath())   // _T("C:\\mobigo\\MobigoSecureRebooter.exe" )
#define     MOBIGO_SECURE_UPGRADER_EXE_PATH                     ((LPCTSTR)CUtilEnv::GetMobigoSecureUpgraderExePath())   // _T("C:\\mobigo\\upgrader\\MobigoSecureUpgrader.exe" )

// etc
#define     MOBIGO_UPGRADE_DOWNLOD_PATH                         ((LPCTSTR)CUtilEnv::GetUpgradeDownloadPath())           // _T("C:\\mobigo\\upgrader\\download")
#define     MOBIGO_UPGRADE_LIST_PATH                            ((LPCTSTR)CUtilEnv::GetUpgradeListPath())               // _T("C:\\mobigo\\upgrader\\mobigo_secure_upgrade_list.json")

// registry
#define     REG_PATH_X64_IE_EMULATION                           _T("SOFTWARE\\WOW6432Node\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_BROWSER_EMULATION")
#define     REG_PATH_X86_IE_EMULATION                           _T("SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_BROWSER_EMULATION")
#define     REG_VALUE_IE_VERSION                                11000 // == IE 11 ( Ref : http://guideme.tistory.com/1 ) 

#define     REG_PATH_MOBIGO_SECURE                              _T("Software\\MobigoSecure")
#define     REG_KEY_CHECK_KEEP_TRAY                             _T("CheckKeepTray"        )
#define     REG_KEY_ROOT_PATH                                   _T("MobigoRootPath"       )

// item id
#define     ITEM_ID_SYSTEM_APK                                  _T("mobigo_systemapp")
#define     ITEM_ID_MOBIGO_SECURE_SETUP                         _T("mobigo_secure_setup")

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////


class AFX_EXT_CLASS CUtilEnv

{
public:
	CUtilEnv(void);
	~CUtilEnv(void);

////////////////////////////////////////////////////////////////////////////////
// member data
////////////////////////////////////////////////////////////////////////////////

protected :
    static CString      m_strMobigoRootPath;

////////////////////////////////////////////////////////////////////////////////
// member function
////////////////////////////////////////////////////////////////////////////////

public :
    static void         Init();
    static void         Exit();

// get/set
public :
    static void         SetMobigoRootPath( CString strMobigoRootPath )   { m_strMobigoRootPath = strMobigoRootPath; }
    static CString      GetMobigoRootPath()                              { return m_strMobigoRootPath;              }
    static CString      GetRootHardDisk();    
    static CString      GetMobigoTempPath();

// exe path 
public : 
    static CString      GetMobigoSecureAppExePath();
    static CString      GetMobigoSecureNothingExePath();
    static CString      GetMobigoSecureUpgraderExePath();
    static CString      GetMobigoSecureKillerExePath();
    static CString      GetMobigoSecureRebooterExePath();

// etc
public :     
    static CString      GetUpgradeListPath();
    static CString      GetUpgradeDownloadPath();

// html
public :   
    static CString      GetAlertHtmlPath();
    static CString      GetConfirmHtmlPath();
};
