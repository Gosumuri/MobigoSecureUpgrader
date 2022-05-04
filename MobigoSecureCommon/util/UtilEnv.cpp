////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "utilenv.h"
#include "UtilOS.h"
#include "UtilRegKey.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// static member
////////////////////////////////////////////////////////////////////////////////

CString             CUtilEnv::m_strMobigoRootPath = "";

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CUtilEnv::CUtilEnv(void)
{
}

CUtilEnv::~CUtilEnv(void)
{
}

////////////////////////////////////////////////////////////////////////////////
// life cycle 
////////////////////////////////////////////////////////////////////////////////

void CUtilEnv::Init()
{
    /** installer에 의해서 설치된 mobigo root path */
    {
        CString strMobigoRootPath = "";
        CUtilRegKey::GetValueStr(HKEY_CURRENT_USER, REG_PATH_MOBIGO_SECURE, REG_KEY_ROOT_PATH, strMobigoRootPath);
        if( strMobigoRootPath.IsEmpty() == TRUE ) {
            m_strMobigoRootPath = MOBIGO_DEFAULT_ROOT_PATH;    
        } else {
            m_strMobigoRootPath = strMobigoRootPath;    
        }
    }
}
    
void CUtilEnv::Exit()
{
}

////////////////////////////////////////////////////////////////////////////////
// path ( get/set )
////////////////////////////////////////////////////////////////////////////////

// "C:\\"
CString CUtilEnv::GetRootHardDisk()
{
    if( 2 < m_strMobigoRootPath.GetLength() ) {
        m_strMobigoRootPath.Mid( 0, 3 );
    }

    return "C:\\";
}

// "c:\\mobigo\\upgrader\\temp"
CString CUtilEnv::GetMobigoTempPath()
{
    static CString strMobigoTempPath = "";
    if( strMobigoTempPath.IsEmpty() == TRUE ) {
        strMobigoTempPath = m_strMobigoRootPath + "\\upgrader\\temp";
    }
    return strMobigoTempPath;
}

////////////////////////////////////////////////////////////////////////////////
// exe path ( get / set ) 
////////////////////////////////////////////////////////////////////////////////

// "C:\\mobigo\\MobigoSecureApp.exe"
CString CUtilEnv::GetMobigoSecureAppExePath()
{
    static CString strAppExePath = "";
    if( strAppExePath.IsEmpty() == TRUE ) {
        strAppExePath = m_strMobigoRootPath + "\\MobigoSecureApp.exe";
    }
    return strAppExePath;
}


// "C:\\mobigo\\MobigoSecureNothing.exe"
CString CUtilEnv::GetMobigoSecureNothingExePath()
{
    static CString strNothingAppExePath = "";
    if( strNothingAppExePath.IsEmpty() == TRUE ) {
        strNothingAppExePath = m_strMobigoRootPath + "\\MobigoSecureNothing.exe";
    }
    return strNothingAppExePath;
}

// "C:\\mobigo\\upgrader\\MobigoSecureUpgrader.exe"
CString CUtilEnv::GetMobigoSecureUpgraderExePath()
{
    static CString strUpgraderAppExePath = "";
    if( strUpgraderAppExePath.IsEmpty() == TRUE ) {
        strUpgraderAppExePath = m_strMobigoRootPath + "\\upgrader\\MobigoSecureUpgrader.exe";
    }
    return strUpgraderAppExePath;
}

// "C:\\mobigo\\MobigoSecureKiller.exe"
CString CUtilEnv::GetMobigoSecureKillerExePath()
{
    static CString strKillerAppExePath = "";
    if( strKillerAppExePath.IsEmpty() == TRUE ) {
        strKillerAppExePath = m_strMobigoRootPath + "\\MobigoSecureKiller.exe";
    }
    return strKillerAppExePath;
}

// "C:\\mobigo\\MobigoSecureRebooter.exe"
CString CUtilEnv::GetMobigoSecureRebooterExePath()
{
    static CString strRebooterAppExePath = "";
    if( strRebooterAppExePath.IsEmpty() == TRUE ) {
        strRebooterAppExePath = m_strMobigoRootPath + "\\MobigoSecureRebooter.exe";
    }
    return strRebooterAppExePath;
}

////////////////////////////////////////////////////////////////////////////////
// html ( get / set ) 
////////////////////////////////////////////////////////////////////////////////

// "C:\\mobigo\\html\\modalPopup\\alert.html"
CString CUtilEnv::GetAlertHtmlPath()
{
    static CString strAlertHtmlPath = "";
    if( strAlertHtmlPath.IsEmpty() == TRUE ) {
        strAlertHtmlPath = m_strMobigoRootPath + "\\html\\modalPopup\\alert.html";
    }
    return strAlertHtmlPath;
}

// "C:\\mobigo\\html\\modalPopup\\confirm.html"
CString CUtilEnv::GetConfirmHtmlPath()
{
    static CString strConfirmHtmlPath = "";
    if( strConfirmHtmlPath.IsEmpty() == TRUE ) {
        strConfirmHtmlPath = m_strMobigoRootPath + "\\html\\modalPopup\\confirm.html";
    }
    return strConfirmHtmlPath;
}

////////////////////////////////////////////////////////////////////////////////
// etc ( get / set ) 
////////////////////////////////////////////////////////////////////////////////

// "C:\\mobigo\\upgrader\\download"
CString CUtilEnv::GetUpgradeDownloadPath()
{
    static CString strUpgradeDownloadPath = "";
    if( strUpgradeDownloadPath.IsEmpty() == TRUE ) {
        strUpgradeDownloadPath = m_strMobigoRootPath + "\\upgrader\\download";
    }
    return strUpgradeDownloadPath;
}

// "C:\\mobigo\\upgrader\\mobigo_secure_upgrade_list.json"
CString CUtilEnv::GetUpgradeListPath()
{
    static CString strUpgradeListPath = "";
    if( strUpgradeListPath.IsEmpty() == TRUE ) {
        strUpgradeListPath = m_strMobigoRootPath + "\\upgrader\\mobigo_secure_upgrade_list.json";
    }
    return strUpgradeListPath;
}

