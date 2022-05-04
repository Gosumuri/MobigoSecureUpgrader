
// PngButtonDlg.cpp : 구현 파일
//

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MobigoSecureUpgrader.h"
#include "SplashDlg.h"
#include "UiSplashView.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSplashDlg, CDialogEx)
	ON_WM_PAINT()
    ON_WM_WINDOWPOSCHANGING()
	ON_WM_QUERYDRAGICON()
    ON_WM_ERASEBKGND()
    ON_WM_NCPAINT()
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CSplashDlg::CSplashDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSplashDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_nCurDownloadSize   = 0;
    m_nTotalDownloadSize = 0;

    m_nZipFileCount      = 0; 
    m_nCurZipTh          = 0;     
}

////////////////////////////////////////////////////////////////////////////////
// virtual
////////////////////////////////////////////////////////////////////////////////

void CSplashDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_MOBIGO_ICON,        m_btMobigoIcon  );
    DDX_Control( pDX, IDC_MOBIGO_LOGO,        m_btMobigoLogo  );
    DDX_Control( pDX, IDC_ST_UPDATE,          m_stUpdate      );
    DDX_Control( pDX, IDC_ST_VERSION,         m_stVersion     );
    DDX_Control( pDX, IDC_ST_MAIN_TEXT,       m_stMainText    );
    DDX_Control( pDX, IDC_ST_MAIN_PERCENT,    m_stMainPercent );


    DDX_Control( pDX, IDC_PG_SPLASH_DOWNLOAD, m_pgDownload    );
    DDX_Control( pDX, IDC_ST_SUB_TEXT,        m_stSubText     );
    DDX_Control( pDX, IDC_ST_BUILD_TIME,      m_stBuildTime   );
}

BOOL CSplashDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

    InitDialog();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

////////////////////////////////////////////////////////////////////////////////
// event handler
////////////////////////////////////////////////////////////////////////////////

void CSplashDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSplashDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CSplashDlg::OnEraseBkgnd( CDC* pDC )
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    CRect rect;
    GetClientRect(&rect);
    CBrush myBrush( RGB( 255, 255, 255)); // dialog background color <- 요기 바꾸면 됨.
    CBrush *pOld = pDC->SelectObject(&myBrush);
    BOOL bRes = pDC->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
    pDC->SelectObject(pOld); // restore old brush

    return bRes; // return CDialogEx::OnEraseBkgnd( pDC );
}

void CSplashDlg::InitDialog()
{
	DEBUG_LOG(">> CSplashDlg::InitControl()");

    CMobigoSecureUpgraderDataMgr*   pAppDataMgr              = ((CMobigoSecureUpgraderApp*)AfxGetApp())->GetDataMgr();
    CUpgradeData*                   pAppUpgradeData          = pAppDataMgr->GetUpgradeData();

    // dialog
    {
        RECT   rectScreen;
        HWND hWndScreen = ::GetDesktopWindow ();
        ::GetWindowRect(hWndScreen,&rectScreen); 
        RECT   rectDialog;
        ::GetWindowRect(this->GetSafeHwnd(),&rectDialog); 
        int nScreenCenterX = ((rectScreen.right-rectScreen.left)/2 -(rectDialog.right-rectDialog.left)/2 );
        int nScreenCenterY = ((rectScreen.bottom-rectScreen.top)/2- (rectDialog.bottom-rectDialog.top)/2 ); 
        ::SetWindowPos( this->GetSafeHwnd(),
                        HWND_NOTOPMOST, 
                        nScreenCenterX, 
                        nScreenCenterY, 
                        (rectDialog.right-rectDialog.left), 
                        (rectDialog.bottom-rectDialog.top), 
                        SWP_NOOWNERZORDER );	
    }

    // Icon
    {
        m_btMobigoIcon.LoadStdImage(IDP_MOBIGO_ICON, _T("PNG"));     //버튼 이미지
        m_btMobigoIcon.LoadAltImage(IDP_MOBIGO_ICON, _T("PNG"));     //버튼 눌렸을때 이미지
        m_btMobigoIcon.EnableToggle(TRUE);                          //버튼 눌렀을때 이미지 바뀌게 할건지
        m_btMobigoIcon.EnableWindow(FALSE);
    }

    // Logo
    {
        m_btMobigoLogo.LoadStdImage(IDP_MOBIGO_LOGO, _T("PNG"));     //버튼 이미지
        m_btMobigoLogo.LoadAltImage(IDP_MOBIGO_LOGO, _T("PNG"));     //버튼 눌렸을때 이미지
        m_btMobigoLogo.EnableToggle(TRUE);                           //버튼 눌렀을때 이미지 바뀌게 할건지
        m_btMobigoLogo.EnableWindow(FALSE);
    }

    // Update 
    {
        m_stUpdate.SetTextColor( RGB(   41,  41,  41)); //Changes the Static Text to Red
        m_stUpdate.SetBkColor(   RGB(  255, 255, 255)); //You probably will not use it, but it's here.
        m_stUpdate.SetWindowText( "업데이트" );
    }

    // Version
    {
        m_stVersion.SetTextColor( RGB(  102, 102, 102)); //Changes the Static Text to Red
        m_stVersion.SetBkColor(   RGB(  255, 255, 255)); //You probably will not use it, but it's here.
        m_stVersion.SetFontSize( 100 );

        CUpgradeItem* pUpgradeItem = pAppUpgradeData->FindUpgradeItemByID( ITEM_ID_MOBIGO_SECURE_SETUP );
        if( pUpgradeItem != NULL ) {
            m_stVersion.SetWindowText( "Ver " + pUpgradeItem->GetVersion() );
        } else {
            m_stVersion.SetWindowText( "" );
        }
    }

    // Main Text
    {
        m_stMainText.SetTextColor( RGB(   41,  41,  41)); //Changes the Static Text to Red
        m_stMainText.SetBkColor(   RGB(  255, 255, 255)); //You probably will not use it, but it's here.
        m_stMainText.SetWindowText( "Ready ... ... ..." );
    }

    // Persent
    {
        m_stMainPercent.SetTextColor( RGB(   10,  109,  232)); //Changes the Static Text to Red
        m_stMainPercent.SetBkColor(   RGB(  255, 255, 255)); //You probably will not use it, but it's here.
        m_stMainPercent.SetWindowText( "0%" );
        m_stMainPercent.SetFontSize( 100 );
    }

    // Progress
    {
	    m_pgDownload.SetRange(0, 500);
	    m_pgDownload.SetStep(20);
	
	    //progress bar의 색 설정. 
	    m_pgDownload.SetForeColour(RGB(10, 109, 232));
	    //progress bar의 배경색  설정.
	    m_pgDownload.SetBkColour(RGB(225, 225, 225));    
	    //progress bar에 의해서 가리워 지지 않았을 때의 Text 색 설정  
	    m_pgDownload.SetTextForeColour(RGB(10, 109, 232));
	    //progress bar에 의해서 가리워 졌을 때의 Text 색 설정  
	    m_pgDownload.SetTextBkColour(RGB(225, 225, 225));   
	
	    //Text 설정 
	    m_pgDownload.SetText( CString("") );
        m_pgDownload.SetShowText( FALSE );
        m_pgDownload.ModifyStyle(0, ~WS_BORDER, ~SWP_DRAWFRAME ); 
    }

    // Sub Text
    {
        m_stSubText.SetTextColor( RGB(  180, 180, 180)); //Changes the Static Text to Red
        m_stSubText.SetBkColor(   RGB(  255, 255, 255)); //You probably will not use it, but it's here.
        m_stSubText.SetWindowText( "Ready ... ... ..." );
    }

    // Build Time
    {
        m_stBuildTime.SetTextColor(  RGB(  180, 180, 180 ));
        m_stBuildTime.SetBkColor(    RGB(  255, 255, 255 )); 

        CString strBuildTime;
        strBuildTime.Format( "Build Time : %s", BUILD_TIME() );
        m_stBuildTime.SetWindowText( strBuildTime );
    }
}

void CSplashDlg::OnWindowPosChanging( WINDOWPOS* lpwndpos )
{
	//-DEBUG_LOG(">> CSplashDlg::OnWindowPosChanging()");
    CDialogEx::OnWindowPosChanging( lpwndpos );

    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
    RECT   rectParent;
    RECT   rectSplash;
    ::GetWindowRect( this->GetParent()->m_hWnd,&rectParent); 
    ::GetWindowRect( this->m_hWnd,&rectSplash); 

    //-DEBUG_LOG("++   rectParent=[%d, %d, %d, %d]", rectParent.left, rectParent.top, rectParent.right, rectParent.bottom );
    //-DEBUG_LOG("++   rectSplash=[%d, %d, %d, %d]", rectSplash.left, rectSplash.top, rectSplash.right, rectSplash.bottom );

    int nCenterX = ((rectParent.right-rectParent.left)/2 - (rectSplash.right-rectSplash.left)/2 );
    int nCenterY = ((rectParent.bottom-rectParent.top)/2 - (rectSplash.bottom-rectSplash.top)/2 ); 

    this->MoveWindow( nCenterX, nCenterY , (rectSplash.right-rectSplash.left), (rectSplash.bottom-rectSplash.top) );
}

////////////////////////////////////////////////////////////////////////////////
// event handler
////////////////////////////////////////////////////////////////////////////////

void CSplashDlg::SetProgressRange( int nLower, int nUpper )
{ 
    m_pgDownload.SetRange( nLower, nUpper );   
}

void CSplashDlg::StepIt()
{
    m_pgDownload.StepIt();
    
    int nLower = m_pgDownload.GetLower();
    int nPos   = m_pgDownload.GetPos();
    int nUpper = m_pgDownload.GetUpper();

	//-DEBUG_LOG("++   nLower = %d", nLower );
	//-DEBUG_LOG("++   nPos   = %d", nPos   );
	//-DEBUG_LOG("++   nUpper = %d", nUpper );

    int nPercent = (int)(((double)(nPos)/((double)nUpper - (double)nLower ))*100.0);

	//-DEBUG_LOG("++   nPercent = %d", nPercent );

    CString strMainText = "";    
    strMainText.Format( "버전 다운로드 중... ... ... " );
	SetMainText( strMainText );

    CString strMainPercent = "";    
    strMainPercent.Format( "%d%%", nPercent );
    SetMainPercent( strMainPercent );
}

void CSplashDlg::StepDownload( int nDownloadDeltaSize )
{
    //-DEBUG_LOG(">> CSplashDlg::StepDownload()" );
    //-DEBUG_LOG("++   nDownloadDeltaSize = %d", nDownloadDeltaSize );

    m_nCurDownloadSize += nDownloadDeltaSize;

    //-DEBUG_LOG("++   m_nCurDownloadSize = %d", m_nCurDownloadSize );
    //-DEBUG_LOG("++   m_nTotalDownloadSize = %d", m_nTotalDownloadSize );
    if( 0 < m_nTotalDownloadSize ) {
        double dCurDownloadPercent = ((double)m_nCurDownloadSize/(double)m_nTotalDownloadSize);
	    //-DEBUG_LOG("++   dCurDownloadPercent = %.2f", dCurDownloadPercent );

        int nLower  = m_pgDownload.GetLower();
        int nUpper  = m_pgDownload.GetUpper();
	    //-DEBUG_LOG("++   nLower = %d", nLower );
	    //-DEBUG_LOG("++   nUpper = %d", nUpper );

        int    nDownloadRange = (double)SPLASH_PROGRESS_POS_DOWNLOAD - (double)SPLASH_PROGRESS_POS_UPGRADE_LIST;
        double dPos = (double)SPLASH_PROGRESS_POS_UPGRADE_LIST + (double)nDownloadRange*(double)dCurDownloadPercent;
	    //-DEBUG_LOG("++   dPos = %.2f", dPos );
        SetProgressPos( (int)dPos );
        int nPercent = (int)(((double)(dPos)/((double)nUpper - (double)nLower))*100.0);
	    //-DEBUG_LOG("++   nPercent = %d", nPercent );

        CString strMainText = "";    
        strMainText.Format( "버전 다운로드 중... ... ... " );
	    SetMainText( strMainText );

        CString strMainPercent = "";    
        strMainPercent.Format( "%d%%", nPercent );
        SetMainPercent( strMainPercent );
    }
}

void CSplashDlg::StepUnzip( int nSubLower, int nSubUpper, int nSubPos )
{
    DEBUG_LOG(">> CSplashDlg::StepDownload()" );
    DEBUG_LOG("++   nSubLower = %d", nSubLower );
    DEBUG_LOG("++   nSubUpper = %d", nSubUpper );
    DEBUG_LOG("++   nSubPos   = %d", nSubPos   );

    int     nLower           = m_pgDownload.GetLower();
    int     nUpper           = m_pgDownload.GetUpper();

    int     nUnzipRange      = (double)SPLASH_PROGRESS_POS_UNZIP - (double)SPLASH_PROGRESS_POS_DOWNLOAD;
    double  dSubUnzipRange   = (double)nUnzipRange/(double)m_nZipFileCount;

    double  dSubUnzipPercent = (double)nSubPos/((double)nSubUpper-(double)nSubLower);
    double  dPos             =    (double)SPLASH_PROGRESS_POS_DOWNLOAD 
                               + (double)dSubUnzipRange*(double)m_nCurZipTh 
                               + (double)dSubUnzipRange*(double)dSubUnzipPercent;

    SetProgressPos( (int)dPos );
    int nPercent = (int)(((double)(dPos)/((double)nUpper - (double)nLower))*100.0);

    CString strMainText = "";    
    strMainText.Format( "드라이버 업데이트 중... ... ... " );
	SetMainText( strMainText );

    CString strMainPercent = "";    
    strMainPercent.Format( "%d%%", nPercent );
    SetMainPercent( strMainPercent );

    if( nUpper == nSubPos ) {
        m_nCurZipTh++;
    }
}

void CSplashDlg::OnNcPaint()
{
    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
    // 그리기 메시지에 대해서는 CDialogEx::OnNcPaint()을(를) 호출하지 마십시오.
	// 테두리 그리기 : http://tansanc.tistory.com/422
    CDC* pDC = GetWindowDC( ); //work out the coordinates of the window rectangle,
    CRect rect; GetWindowRect( &rect); 
    rect.OffsetRect( -rect.left, -rect.top); //Draw a single line around the outside
    CBrush brush( RGB( 10, 109, 232));
    pDC->FrameRect( &rect, &brush); 
    ReleaseDC( pDC);
}
