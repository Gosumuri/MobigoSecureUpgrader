// MobigoSecureUpgraderView.cpp : CFirstStepSplash 클래스의 구현
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
	// TODO: 여기에 생성 코드를 추가합니다.
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

    // 1. Tray Setting 값 저장 :: tray를 설정한 경우 kill이 안됨.. 
    // - 일단 기존 값 저장 후 무조건 false로 설정뒤 다시 기존값 설정
    // - [주의] 강제로 종료 시킬 수도 있지만, 그렇게 되면, tray icon 잔상이 남는 이슈가 있다. 
    CUtilRegKey::GetValueDword(HKEY_CURRENT_USER, REG_PATH_MOBIGO_SECURE, REG_KEY_CHECK_KEEP_TRAY, dwReadValue);
    CUtilRegKey::SetValueDword(HKEY_CURRENT_USER, REG_PATH_MOBIGO_SECURE, REG_KEY_CHECK_KEEP_TRAY, FALSE);
    
    // 2. kill process
    {
        // 2.1. release용일 경우의 main frame의 이름을 찾는다.
        HWND hWndSmartMobigoApp = CUtilSys::FindAllWindowByPartialName( MOBIGO_SECURE_APP_MAIN_FRAME_NAME ); 
        if( hWndSmartMobigoApp == NULL ) {
            // 2.2. debug용일 경우의 main frame의 이름을 찾는다.
            hWndSmartMobigoApp = CUtilSys::FindAllWindowByPartialName( MOBIGO_SECURE_APP_NAME ); 
        }

        // 2.2. 스스로 죽도록 유도한다. 
        if ( hWndSmartMobigoApp != NULL) {
            ::SendMessage( hWndSmartMobigoApp, WM_QUIT, NULL, NULL );       
        // 2.3. 강제로 종료 시킨다. 
        } else {
            CString strAppName = CUtilFile::ExtractFileName(MOBIGO_SECURE_APP_EXE_PATH);
	        DEBUG_LOG("++   CUtilSys::KillProcessByName( \"" + strAppName + "\" )" );
            CUtilSys::KillProcessByName( strAppName  );
        }
    }
    // 3.  Setting 다시 저장
    CUtilRegKey::SetValueDword(HKEY_CURRENT_USER, REG_PATH_MOBIGO_SECURE, REG_KEY_CHECK_KEEP_TRAY, dwReadValue);
}