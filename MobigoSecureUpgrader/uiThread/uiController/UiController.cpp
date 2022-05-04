
// ChildFrm.cpp : CUiController 클래스의 구현
//

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MobigoSecureUpgrader.h"
#include "MobigoSecureUpgraderDataMgr.h"

#include "UiController.h"
#include "UiDebugView.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

#define MY_TIMER_SHOW_RELEASE_VIEW      1

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CUiController, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CUiController, CMDIChildWnd)
	ON_MESSAGE( MY_MSG_SHOW_RELEASE_VIEW,               OnShowReleaseView               )

	ON_MESSAGE( MY_MSG_U_COMPLETE_FIRST_STEP,           OnCompleteFirstStep             )
	ON_MESSAGE( MY_MSG_U_COMPLETE_SECOND_STEP,          OnCompleteSecondStep            )
	ON_MESSAGE( MY_MSG_U_COMPLETE_THIRD_STEP,           OnCompleteThirdStep             )

	ON_MESSAGE( MY_MSG_U_COMPLETE_DOWNLOAD_EACH_ITEM,   OnCompleteDownloadEachItem      )
	ON_MESSAGE( MY_MSG_U_COMPLETE_UNZIP_EACH_ITEM,      OnCompleteUnzipEachItem         )
    ON_WM_TIMER()
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// static 
////////////////////////////////////////////////////////////////////////////////

CUiReleaseView CUiController::m_uiReleaseView;
BOOL           CUiController::m_bShownReleaseView = FALSE;  

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CUiController::CUiController()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CUiController::~CUiController()
{
}

////////////////////////////////////////////////////////////////////////////////
// life cycle 
////////////////////////////////////////////////////////////////////////////////

void	CUiController::Init()
{
	DEBUG_LOG(">> CUiController::Init()");
}

void	CUiController::Exit()
{
	DEBUG_LOG(">> CUiController::Exit()");
}

////////////////////////////////////////////////////////////////////////////////
// virtual function 
////////////////////////////////////////////////////////////////////////////////

BOOL CUiController::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

void CUiController::ActivateFrame(int nCmdShow)
{
	// TODO: Add your specialized code here and/or call the base class
    SET_UI_CONTROLLER( this );
	nCmdShow = SW_MAXIMIZE;

#ifdef FEATURE_RELEASE_VIEW
	DEBUG_LOG("++   m_bShownReleaseView=%s", B2S(m_bShownReleaseView) );
    if( m_bShownReleaseView == FALSE ) {
        m_bShownReleaseView = TRUE;
	    DEBUG_LOG("++   m_bShownReleaseView=%s", B2S(m_bShownReleaseView) );
        SetTimer( MY_TIMER_SHOW_RELEASE_VIEW, 500, NULL ); 
    }
#else 
    // debug 모드일 경우에는, View가 활성화 때 호출되는 CUiDebugView::OnWindowPosChanging()에서 splash를 초기화 한다.
#endif

	CMDIChildWnd::ActivateFrame(nCmdShow);
}
////////////////////////////////////////////////////////////////////////////////
// debug function 
////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void CUiController::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CUiController::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}
#endif //_DEBUG

////////////////////////////////////////////////////////////////////////////////
// public handler
///////////////////////////////////////////////////////////////////////////////

LRESULT CUiController::OnShowReleaseView(WPARAM wParam, LPARAM lParam)
{
	DEBUG_LOG(">> CUiController::OnShowReleaseView()");

	m_uiReleaseView.DoModal();
    QUIT_APP(-1);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
// event handler( 1 ~ 4 step)
///////////////////////////////////////////////////////////////////////////////

// [1] step -> [2] step
LRESULT CUiController::OnCompleteFirstStep(WPARAM wParam, LPARAM lParam)
{
    DEBUG_LOG(">> CUiController::OnCompleteFirstStep()");

    // Second Step Splash
#ifdef FEATURE_RELEASE_VIEW
    m_uiReleaseView.SecondStepSplash();
#else
    ((CUiDebugView*)GetActiveView())->SecondStepSplash();
#endif

    return TRUE;
}

// [2] step -> [3] step
LRESULT CUiController::OnCompleteSecondStep(WPARAM wParam, LPARAM lParam)
{
    DEBUG_LOG(">> CUiController::OnCompleteSecondStep()");

#ifdef FEATURE_RELEASE_VIEW
    m_uiReleaseView.ThirdStepSplash();
#else
    ((CUiDebugView*)GetActiveView())->ThirdStepSplash();
#endif

    return TRUE;
}

// [3] step -> [4] step
LRESULT CUiController::OnCompleteThirdStep(WPARAM wParam, LPARAM lParam)
{
    DEBUG_LOG(">> CUiController::OnCompleteThirdStep()");

#ifdef FEATURE_RELEASE_VIEW
    m_uiReleaseView.FourthStepSplash();
#else
    ((CUiDebugView*)GetActiveView())->FourthStepSplash();
#endif
    
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
// event handler(each item )
///////////////////////////////////////////////////////////////////////////////

LRESULT CUiController::OnCompleteDownloadEachItem(WPARAM wParam, LPARAM lParam)
{
	DEBUG_LOG(">> CUiController::OnCompleteDownloadEachItem()" );

    CDataMgr    *pDataMgr           = (CDataMgr*)GET_DATA_MGR();
    CString     strDownloadUrl      = CString( (LPCSTR)wParam );
	DEBUG_LOG("++   strDownloadUrl=" + strDownloadUrl );

    CUpgradeData *pUpgradeData = pDataMgr->GetUpgradeData();

    CUpgradeItem *pUpgradeItem = pUpgradeData->FindUpgradeItemByDownloadUrl( strDownloadUrl );
    if( pUpgradeItem != NULL ) {
        pUpgradeItem->SetDownloadComplete( TRUE );
    }

    return TRUE;
}

LRESULT CUiController::OnCompleteUnzipEachItem(WPARAM wParam, LPARAM lParam)
{
	DEBUG_LOG(">> CUiController::OnCompleteUnzipEachItem()" );

    CDataMgr    *pDataMgr           = (CDataMgr*)GET_DATA_MGR();
    CString     strZipPath          = CString( (LPCSTR)wParam );

    strZipPath.Replace( '\\', '/' );
	DEBUG_LOG("++   strZipPath=" + strZipPath );

    CUpgradeData *pUpgradeData = pDataMgr->GetUpgradeData();

    CUpgradeItem *pUpgradeItem = pUpgradeData->FindUpgradeItemBySavePath( strZipPath );
    if( pUpgradeItem != NULL ) {
        pUpgradeItem->SetUnzipComplete( TRUE );
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
// timer
///////////////////////////////////////////////////////////////////////////////

void CUiController::OnTimer( UINT_PTR nIDEvent )
{
	DEBUG_LOG(">> CUiController::OnTimer()");

    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    switch( nIDEvent ) {
        case MY_TIMER_SHOW_RELEASE_VIEW : {
            KillTimer(MY_TIMER_SHOW_RELEASE_VIEW);    
            PostMessage( MY_MSG_SHOW_RELEASE_VIEW, NULL, NULL );
        }
        break;
    }
    
    CMDIChildWnd::OnTimer( nIDEvent );
}


////////////////////////////////////////////////////////////////////////////////
// utility
///////////////////////////////////////////////////////////////////////////////
