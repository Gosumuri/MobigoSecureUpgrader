////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MobigoSecureUpgrader.h"
#include "resource.h"
#include "UiReleaseView.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CUiReleaseView, CDialogEx)

BEGIN_MESSAGE_MAP(CUiReleaseView, CDialogEx)
    ON_WM_WINDOWPOSCHANGING()
    ON_WM_TIMER()
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CUiReleaseView::CUiReleaseView(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUiReleaseView::IDD, pParent)
{
}

CUiReleaseView::~CUiReleaseView()
{
}

////////////////////////////////////////////////////////////////////////////////
// virtual
////////////////////////////////////////////////////////////////////////////////

void CUiReleaseView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CUiReleaseView::OnInitDialog()
{
    DEBUG_LOG( ">> CUiReleaseView::OnInitDialog()" );

	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

    // 1. set title 
    CString strTitle;
    strTitle.Format( "%s( %s )", MOBIGO_SECURE_UPGRADER_NAME, BUILD_TIME() );
    SetWindowText( strTitle ); 

    // 2. set icon 
    HICON hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon, TRUE);		// 큰 아이콘을 설정합니다.
	SetIcon(hIcon, FALSE);		// 작은 아이콘을 설정합니다.

    // 3. this를 넘기는 이유는 CUiSplashView에서 window handle을 사용하기 위함.
    ShowSplash( this, 0, SPLASH_PROGRESS_POS_UNZIP );
    FirstStepSplash();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

////////////////////////////////////////////////////////////////////////////////
// event handler
////////////////////////////////////////////////////////////////////////////////

void CUiReleaseView::OnWindowPosChanging( WINDOWPOS* lpwndpos )
{
    CDialogEx::OnWindowPosChanging( lpwndpos );

    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
    ModifyStyle (WS_CAPTION, 0); // to hide
    RECT   rectScreen;
    RECT   rectSplash;
    HWND hWndScreen = ::GetDesktopWindow ();
    ::GetWindowRect(hWndScreen,&rectScreen); 
    ::GetWindowRect(m_pSplashDlg->m_hWnd, &rectSplash); 

    int nScreenCenterX = ((rectScreen.right-rectScreen.left)/2- (rectSplash.right-rectSplash.left)/2 );
    int nScreenCenterY = ((rectScreen.bottom-rectScreen.top)/2- (rectSplash.bottom-rectSplash.top)/2 ); 

#if 1 // upgrade중에 다른 프로그램을 사용할 수 있어야 한다는 요구사항으로 인해서 TOPMOST제거( 20180222_jkim )
    ::SetWindowPos(this->GetSafeHwnd(), HWND_NOTOPMOST, nScreenCenterX, nScreenCenterY , (rectSplash.right-rectSplash.left), (rectSplash.bottom-rectSplash.top), SWP_NOOWNERZORDER);	
#else 
    ::SetWindowPos(this->GetSafeHwnd(), HWND_TOPMOST, nScreenCenterX, nScreenCenterY , (rectSplash.right-rectSplash.left), (rectSplash.bottom-rectSplash.top), SWP_NOOWNERZORDER);	
#endif

#ifdef FEATURE_RUN_BACKGROUND
    if( CStepSplash::IsBackground() == TRUE ) {
        if( lpwndpos != NULL ) {
            lpwndpos->flags &= ~SWP_SHOWWINDOW;
        }
    }
#endif
}

void CUiReleaseView::OnTimer( UINT_PTR nIDEvent )
{
	DEBUG_LOG(">> CUiReleaseView::OnTimer()");

    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    switch( nIDEvent ) {
        case MY_TIMER_SPLASH_CLOSE : {
            KillTimer(MY_TIMER_SPLASH_CLOSE);    
            OnCloseSplash();
            OnOK();
        }
        break;
    }
    
    CDialogEx::OnTimer( nIDEvent );
}
