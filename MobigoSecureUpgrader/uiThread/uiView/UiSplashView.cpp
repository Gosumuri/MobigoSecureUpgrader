// MobigoSecureUpgraderView.cpp : CUiSplashView Ŭ������ ����
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
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

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
// [0�ܰ�] splash�� ȭ�鿡 ǥ���Ѵ�. 
////////////////////////////////////////////////////////////////////////////////

void CUiSplashView::ShowSplash( CWnd* pWndParent, int nLower, int nUpper )
{
	DEBUG_LOG(">> CUiSplashView::ShowSplash()");
    DEBUG_LOG( "/**************************************************************************************************/" );
    DEBUG_LOG( "/*                                                                                                  " ); 
    DEBUG_LOG( "/*  [0�ܰ�] splash�� ȭ�鿡 ǥ���Ѵ�.                                                               " ); 
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
// [1�ܰ�] upgrade list�� ������ ���� �о� ����, ������ up �Ǿ��� �� check�Ѵ�.
////////////////////////////////////////////////////////////////////////////////

void CUiSplashView::FirstStepSplash()
{
    DEBUG_LOG( "/**************************************************************************************************/" );
    DEBUG_LOG( "/*                                                                                                  " ); 
    DEBUG_LOG( "/*  [1�ܰ�] upgrade list�� ������ ���� �о� ����, ������ up �Ǿ��� �� check�Ѵ�.                    " ); 
    DEBUG_LOG( "/*                                                                                                  " ); 
    DEBUG_LOG( "/**************************************************************************************************/" );

	DEBUG_LOG(">> CUiSplashView::FirstStepSplash()");
    m_firstStepSplash.Step();
}

////////////////////////////////////////////////////////////////////////////////
// [2�ܰ�] Upgrade�� �ʿ��� item�� Download �޴´�.
////////////////////////////////////////////////////////////////////////////////

void CUiSplashView::SecondStepSplash()
{
    DEBUG_LOG( "/**************************************************************************************************/" );
    DEBUG_LOG( "/*                                                                                                  " ); 
    DEBUG_LOG( "/*  [2�ܰ�] Upgrade�� �ʿ��� item�� Download �޴´�.                                                " ); 
    DEBUG_LOG( "/*                                                                                                  " ); 
    DEBUG_LOG( "/**************************************************************************************************/" );

	DEBUG_LOG(">> CUiSplashView::SecondStepSplash()");

    m_secondStepSplash.Step();
}


////////////////////////////////////////////////////////////////////////////////
// [3�ܰ�] download ���� item�� zip�� ������ �����Ѵ�. 
////////////////////////////////////////////////////////////////////////////////

void CUiSplashView::ThirdStepSplash()
{
    DEBUG_LOG( "/**************************************************************************************************/" );
    DEBUG_LOG( "/*                                                                                                  " ); 
    DEBUG_LOG( "/*  [3�ܰ�] download ���� item�� zip�� ������ �����Ѵ�.                                             " ); 
    DEBUG_LOG( "/*                                                                                                  " ); 
    DEBUG_LOG( "/**************************************************************************************************/" );

	DEBUG_LOG(">> CUiSplashView::ThirdStepSplash()");

    m_thirdStepSplash.Step();
}

////////////////////////////////////////////////////////////////////////////////
// [4�ܰ�] 100%�� ǥ���ϰ�, Splash â�� �ݴ´�.
////////////////////////////////////////////////////////////////////////////////

void CUiSplashView::FourthStepSplash()
{
    DEBUG_LOG( "/**************************************************************************************************/" );
    DEBUG_LOG( "/*                                                                                                  " ); 
    DEBUG_LOG( "/*  [4�ܰ�] 100�� ǥ���ϰ�, Splash â�� �ݴ´�.                                                     " ); 
    DEBUG_LOG( "/*                                                                                                  " ); 
    DEBUG_LOG( "/**************************************************************************************************/" );

	DEBUG_LOG(">> CUiSplashView::FourthStepSplash()");

	m_pSplashDlg->SetMainText("���� ������Ʈ �Ϸ� ... ... ... ");
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

        // 2.1. mobigo secure app�� kill ���Ѿ� ��ġ�� �����ϴ�. 
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
