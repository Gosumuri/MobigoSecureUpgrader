// MobigoSecureUpgraderView.cpp : CFirstStepSplash Ŭ������ ����
//

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StepSplash.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// static 
////////////////////////////////////////////////////////////////////////////////

BOOL CStepSplash::m_bBackground = TRUE;

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CStepSplash::CStepSplash()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
}

CStepSplash::~CStepSplash()
{
}

////////////////////////////////////////////////////////////////////////////////
// static 
/////////////////////////////////////////////////////////////////////////////

void CStepSplash::KillMobigoSecureApp()
{
	DEBUG_LOG(">> CStepSplash::KillMobigoSecureApp()");

    DWORD                     dwReadValue     = TRUE;

    // 1. Tray Setting �� ���� :: tray�� ������ ��� kill�� �ȵ�.. 
    // - �ϴ� ���� �� ���� �� ������ false�� ������ �ٽ� ������ ����
    // - [����] ������ ���� ��ų ���� ������, �׷��� �Ǹ�, tray icon �ܻ��� ���� �̽��� �ִ�. 
    CUtilRegKey::GetValueDword(HKEY_CURRENT_USER, REG_PATH_MOBIGO_SECURE, REG_KEY_CHECK_KEEP_TRAY, dwReadValue);
    CUtilRegKey::SetValueDword(HKEY_CURRENT_USER, REG_PATH_MOBIGO_SECURE, REG_KEY_CHECK_KEEP_TRAY, FALSE);
    
    // 2. kill process
    {
        // 2.1. release���� ����� main frame�� �̸��� ã�´�.
        HWND hWndSmartMobigoApp = CUtilSys::FindAllWindowByPartialName( MOBIGO_SECURE_APP_MAIN_FRAME_NAME ); 
        if( hWndSmartMobigoApp == NULL ) {
            // 2.2. debug���� ����� main frame�� �̸��� ã�´�.
            hWndSmartMobigoApp = CUtilSys::FindAllWindowByPartialName( MOBIGO_SECURE_APP_NAME ); 
        }

        // 2.2. ������ �׵��� �����Ѵ�. 
        if ( hWndSmartMobigoApp != NULL) {
            ::SendMessage( hWndSmartMobigoApp, WM_QUIT, NULL, NULL );       
        // 2.3. ������ ���� ��Ų��. 
        } else {
            CString strAppName = CUtilFile::ExtractFileName(MOBIGO_SECURE_APP_EXE_PATH);
	        DEBUG_LOG("++   CUtilSys::KillProcessByName( \"" + strAppName + "\" )" );
            CUtilSys::KillProcessByName( strAppName  );
        }
    }
    // 3.  Setting �ٽ� ����
    CUtilRegKey::SetValueDword(HKEY_CURRENT_USER, REG_PATH_MOBIGO_SECURE, REG_KEY_CHECK_KEEP_TRAY, dwReadValue);
}