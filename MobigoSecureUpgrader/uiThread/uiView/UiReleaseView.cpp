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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

    // 1. set title 
    CString strTitle;
    strTitle.Format( "%s( %s )", MOBIGO_SECURE_UPGRADER_NAME, BUILD_TIME() );
    SetWindowText( strTitle ); 

    // 2. set icon 
    HICON hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon, TRUE);		// ū �������� �����մϴ�.
	SetIcon(hIcon, FALSE);		// ���� �������� �����մϴ�.

    // 3. this�� �ѱ�� ������ CUiSplashView���� window handle�� ����ϱ� ����.
    ShowSplash( this, 0, SPLASH_PROGRESS_POS_UNZIP );
    FirstStepSplash();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

////////////////////////////////////////////////////////////////////////////////
// event handler
////////////////////////////////////////////////////////////////////////////////

void CUiReleaseView::OnWindowPosChanging( WINDOWPOS* lpwndpos )
{
    CDialogEx::OnWindowPosChanging( lpwndpos );

    // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
    ModifyStyle (WS_CAPTION, 0); // to hide
    RECT   rectScreen;
    RECT   rectSplash;
    HWND hWndScreen = ::GetDesktopWindow ();
    ::GetWindowRect(hWndScreen,&rectScreen); 
    ::GetWindowRect(m_pSplashDlg->m_hWnd, &rectSplash); 

    int nScreenCenterX = ((rectScreen.right-rectScreen.left)/2- (rectSplash.right-rectSplash.left)/2 );
    int nScreenCenterY = ((rectScreen.bottom-rectScreen.top)/2- (rectSplash.bottom-rectSplash.top)/2 ); 

#if 1 // upgrade�߿� �ٸ� ���α׷��� ����� �� �־�� �Ѵٴ� �䱸�������� ���ؼ� TOPMOST����( 20180222_jkim )
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

    // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
