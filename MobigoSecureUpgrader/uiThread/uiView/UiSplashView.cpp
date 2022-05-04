// MobigoSecureUpgraderView.cpp : CUiSplashView 클래스의 구현
//

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uisplashview.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CUiSplashView::CUiSplashView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

    m_pWndParent        = NULL;
    m_pSplashDlg        = NULL;
}

CUiSplashView::~CUiSplashView()
{
}

////////////////////////////////////////////////////////////////////////////////
// virtual
/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// debug
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// event handler
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// [0단계] splash를 화면에 표시한다. 
////////////////////////////////////////////////////////////////////////////////

void CUiSplashView::ShowSplash( CWnd* pWndParent, int nLower, int nUpper )
{
	DEBUG_LOG(">> CUiSplashView::ShowSplash()");
    DEBUG_LOG( "/**************************************************************************************************/" );
    DEBUG_LOG( "/*                                                                                                  " ); 
    DEBUG_LOG( "/*  [0단계] splash를 화면에 표시한다.                                                               " ); 
    DEBUG_LOG( "/*                                                                                                  " ); 
    DEBUG_LOG( "/**************************************************************************************************/" );

	DEBUG_LOG(">> CUiSplashView::ShowSplash()");
	DEBUG_LOG("++   nLower=%d", nLower );
	DEBUG_LOG("++   nUpper=%d", nUpper );

    // splash dialog
    m_pWndParent = pWndParent;

    if( m_pSplashDlg == NULL ) {
        m_pSplashDlg = new CSplashDlg();
        m_pSplashDlg->Create( CSplashDlg::IDD, m_pWndParent );
	    m_pSplashDlg->ShowWindow(SW_SHOW);
    }
    m_pSplashDlg->InitDialog();
    m_pSplashDlg->SetProgressRange( nLower, nUpper );

    // first step
    m_firstStepSplash.SetSplashDlg( m_pSplashDlg );
    m_firstStepSplash.SetParentWnd( m_pWndParent );

    // second step
    m_secondStepSplash.SetSplashDlg( m_pSplashDlg );
    m_secondStepSplash.SetParentWnd( m_pWndParent );

    // third step
    m_thirdStepSplash.SetSplashDlg( m_pSplashDlg );
    m_thirdStepSplash.SetParentWnd( m_pWndParent );

}

////////////////////////////////////////////////////////////////////////////////
// [1단계] upgrade list를 서버로 부터 읽어 오고, 버전이 up 되었는 지 check한다.
////////////////////////////////////////////////////////////////////////////////

void CUiSplashView::FirstStepSplash()
{
    DEBUG_LOG( "/**************************************************************************************************/" );
    DEBUG_LOG( "/*                                                                                                  " ); 
    DEBUG_LOG( "/*  [1단계] upgrade list를 서버로 부터 읽어 오고, 버전이 up 되었는 지 check한다.                    " ); 
    DEBUG_LOG( "/*                                                                                                  " ); 
    DEBUG_LOG( "/**************************************************************************************************/" );

	DEBUG_LOG(">> CUiSplashView::FirstStepSplash()");
    m_firstStepSplash.Step();
}

////////////////////////////////////////////////////////////////////////////////
// [2단계] Upgrade가 필요한 item을 Download 받는다.
////////////////////////////////////////////////////////////////////////////////

void CUiSplashView::SecondStepSplash()
{
    DEBUG_LOG( "/**************************************************************************************************/" );
    DEBUG_LOG( "/*                                                                                                  " ); 
    DEBUG_LOG( "/*  [2단계] Upgrade가 필요한 item을 Download 받는다.                                                " ); 
    DEBUG_LOG( "/*                                                                                                  " ); 
    DEBUG_LOG( "/**************************************************************************************************/" );

	DEBUG_LOG(">> CUiSplashView::SecondStepSplash()");

    m_secondStepSplash.Step();
}


////////////////////////////////////////////////////////////////////////////////
// [3단계] download 받은 item은 zip은 압축을 해제한다. 
////////////////////////////////////////////////////////////////////////////////

void CUiSplashView::ThirdStepSplash()
{
    DEBUG_LOG( "/**************************************************************************************************/" );
    DEBUG_LOG( "/*                                                                                                  " ); 
    DEBUG_LOG( "/*  [3단계] download 받은 item은 zip은 압축을 해제한다.                                             " ); 
    DEBUG_LOG( "/*                                                                                                  " ); 
    DEBUG_LOG( "/**************************************************************************************************/" );

	DEBUG_LOG(">> CUiSplashView::ThirdStepSplash()");

    m_thirdStepSplash.Step();
}

////////////////////////////////////////////////////////////////////////////////
// [4단계] 100%를 표시하고, Splash 창을 닫는다.
////////////////////////////////////////////////////////////////////////////////

void CUiSplashView::FourthStepSplash()
{
    DEBUG_LOG( "/**************************************************************************************************/" );
    DEBUG_LOG( "/*                                                                                                  " ); 
    DEBUG_LOG( "/*  [4단계] 100를 표시하고, Splash 창을 닫는다.                                                     " ); 
    DEBUG_LOG( "/*                                                                                                  " ); 
    DEBUG_LOG( "/**************************************************************************************************/" );

	DEBUG_LOG(">> CUiSplashView::FourthStepSplash()");

	m_pSplashDlg->SetMainText("버전 업데이트 완료 ... ... ... ");
	m_pSplashDlg->SetMainPercent("100%");
	m_pSplashDlg->SetProgressPos( SPLASH_PROGRESS_POS_UNZIP );

	// close
    m_pWndParent->SetTimer( MY_TIMER_SPLASH_CLOSE, 1000, NULL ); 
}

////////////////////////////////////////////////////////////////////////////////
// OnClose
///////////////////////////////////////////////////////////////////////////////

void CUiSplashView::OnCloseSplash( BOOL bRunApp, BOOL bExit )
{
	DEBUG_LOG(">> CUiSplashView::OnCloseSplash()");

    CDataMgr*           pDataMgr      = (CDataMgr*)GET_DATA_MGR();

    // 1. hide splash
    m_pSplashDlg->EndDialog(IDOK);

    // 2. run mobigo secure app
    if( bRunApp == TRUE ) {
        BOOL                      bRunSetupExe    = FALSE;

        // 2.1. mobigo secure app을 kill 시켜야 설치가 가능하다. 
        CStepSplash::KillMobigoSecureApp();

        // 2.2. kill process
        CString strAppName = CUtilFile::ExtractFileName(MOBIGO_SECURE_APP_EXE_PATH);
	    DEBUG_LOG("++   CUtilSys::KillProcessByName( \"" + strAppName + "\" )" );
        CUtilSys::KillProcessByName( strAppName  );
        
        // 2.4. run "app.exe"
        if( bRunSetupExe == FALSE ) {
            if( CUtilFile::IsExistPath( MOBIGO_SECURE_APP_EXE_PATH ) == TRUE ) {
	            DEBUG_LOG("++   CUtilSys::Run( \"%s\" , \"\", \"%s\", FALSE )", MOBIGO_SECURE_APP_EXE_PATH, MOBIGO_ROOT_PATH );
                CUtilSys::Run( MOBIGO_SECURE_APP_EXE_PATH, "", MOBIGO_ROOT_PATH, FALSE );
            } else {
                ERROR_LOG("++   [ERROR] Not Exist App Exe =%s", MOBIGO_SECURE_APP_EXE_PATH );
            }
        }
    }

    // 3. program exit
    if( bExit == TRUE ) {
	    DEBUG_LOG("++   QUIT_APP(-1)" );
        QUIT_APP(-1);
        return;
    }
}

////////////////////////////////////////////////////////////////////////////////
// private 
///////////////////////////////////////////////////////////////////////////////
