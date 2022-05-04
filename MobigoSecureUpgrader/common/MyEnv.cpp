////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "myenv.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// static member
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CMyEnv::CMyEnv(void)
{
}

CMyEnv::~CMyEnv(void)
{
}

////////////////////////////////////////////////////////////////////////////////
// life cycle 
////////////////////////////////////////////////////////////////////////////////

void	CMyEnv::Init()
{
    CUtilEnv::Init();

    /** 최신 웹브라우저 컨트롤 버전을 사용하도록 설정. http://guideme.tistory.com/1 */
    {
        if (CUtilOS::Is64BitOS() == TRUE ) {
            CUtilRegKey::SetValueDword(HKEY_LOCAL_MACHINE, REG_PATH_X64_IE_EMULATION, MOBIGO_SECURE_UPGRADER_EXE, REG_VALUE_IE_VERSION);
        } else {
            CUtilRegKey::SetValueDword(HKEY_LOCAL_MACHINE, REG_PATH_X86_IE_EMULATION, MOBIGO_SECURE_UPGRADER_EXE, REG_VALUE_IE_VERSION);
        }
    }
}

void	CMyEnv::Exit()
{
    CUtilEnv::Exit();

    // 바탕화면에 App ShortCut 추가
    CUtilSys::CreateDesktopShortcut( (LPTSTR)MOBIGO_SECURE_APP_EXE_PATH, MOBIGO_SECURE_APP_NAME );
#if 0 // upgrader는 노출 대상이 아니므로 short-cut을 만들지 않는다( 20180219_jkim )
    // 바탕화면에 Upgrader ShortCut 추가
    CUtilSys::CreateDesktopShortcut( (LPTSTR)UPGRADER_APP_EXE_PATH, MOBIGO_SECURE_UPGRADER_NAME );
#endif
}


////////////////////////////////////////////////////////////////////////////////
// get / set
////////////////////////////////////////////////////////////////////////////////

// "c:\\mobigo\\upgrader\\mobigo_secure_up_config.xml"
CString CMyEnv::GetMobigoSecureUpgraderConfigPath()
{
    static CString strMobigoSecureUpgraderConfigPath = "";
    if (strMobigoSecureUpgraderConfigPath.IsEmpty() == TRUE) {
        strMobigoSecureUpgraderConfigPath = m_strMobigoRootPath + "\\upgrader\\mobigo_secure_up_config.xml";
    }
    return strMobigoSecureUpgraderConfigPath;
}

// "c:\\mobigo\\upgrader\\mobigo_secure_up_workspace.xml"
CString CMyEnv::GetMobigoSecureUpgraderWorkspacePath()
{
    static CString strMobigoSecureUpgraderWorkspacePath = "";
    if (strMobigoSecureUpgraderWorkspacePath.IsEmpty() == TRUE) {
        strMobigoSecureUpgraderWorkspacePath = m_strMobigoRootPath + "\\upgrader\\mobigo_secure_up_workspace.xml";
    }
    return strMobigoSecureUpgraderWorkspacePath;
}

// "c:\\mobigo\\upgrader\\mobigo_secure_up_file.xml"
CString CMyEnv::GetMobigoSecureUpgraderFileTreePath()
{
    static CString strMobigoSecureUpgraderFileTreePath = "";
    if (strMobigoSecureUpgraderFileTreePath.IsEmpty() == TRUE) {
        strMobigoSecureUpgraderFileTreePath = m_strMobigoRootPath + "\\upgrader\\mobigo_secure_up_file.xml";
    }
    return strMobigoSecureUpgraderFileTreePath;
}