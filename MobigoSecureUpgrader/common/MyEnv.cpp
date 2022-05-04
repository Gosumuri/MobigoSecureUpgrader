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

    /** �ֽ� �������� ��Ʈ�� ������ ����ϵ��� ����. http://guideme.tistory.com/1 */
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

    // ����ȭ�鿡 App ShortCut �߰�
    CUtilSys::CreateDesktopShortcut( (LPTSTR)MOBIGO_SECURE_APP_EXE_PATH, MOBIGO_SECURE_APP_NAME );
#if 0 // upgrader�� ���� ����� �ƴϹǷ� short-cut�� ������ �ʴ´�( 20180219_jkim )
    // ����ȭ�鿡 Upgrader ShortCut �߰�
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