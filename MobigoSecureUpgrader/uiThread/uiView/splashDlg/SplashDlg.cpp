
// PngButtonDlg.cpp : ���� ����
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

    InitDialog();

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

////////////////////////////////////////////////////////////////////////////////
// event handler
////////////////////////////////////////////////////////////////////////////////

void CSplashDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CSplashDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CSplashDlg::OnEraseBkgnd( CDC* pDC )
{
    // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
    CRect rect;
    GetClientRect(&rect);
    CBrush myBrush( RGB( 255, 255, 255)); // dialog background color <- ��� �ٲٸ� ��.
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
        m_btMobigoIcon.LoadStdImage(IDP_MOBIGO_ICON, _T("PNG"));     //��ư �̹���
        m_btMobigoIcon.LoadAltImage(IDP_MOBIGO_ICON, _T("PNG"));     //��ư �������� �̹���
        m_btMobigoIcon.EnableToggle(TRUE);                          //��ư �������� �̹��� �ٲ�� �Ұ���
        m_btMobigoIcon.EnableWindow(FALSE);
    }

    // Logo
    {
        m_btMobigoLogo.LoadStdImage(IDP_MOBIGO_LOGO, _T("PNG"));     //��ư �̹���
        m_btMobigoLogo.LoadAltImage(IDP_MOBIGO_LOGO, _T("PNG"));     //��ư �������� �̹���
        m_btMobigoLogo.EnableToggle(TRUE);                           //��ư �������� �̹��� �ٲ�� �Ұ���
        m_btMobigoLogo.EnableWindow(FALSE);
    }

    // Update 
    {
        m_stUpdate.SetTextColor( RGB(   41,  41,  41)); //Changes the Static Text to Red
        m_stUpdate.SetBkColor(   RGB(  255, 255, 255)); //You probably will not use it, but it's here.
        m_stUpdate.SetWindowText( "������Ʈ" );
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
	
	    //progress bar�� �� ����. 
	    m_pgDownload.SetForeColour(RGB(10, 109, 232));
	    //progress bar�� ����  ����.
	    m_pgDownload.SetBkColour(RGB(225, 225, 225));    
	    //progress bar�� ���ؼ� ������ ���� �ʾ��� ���� Text �� ����  
	    m_pgDownload.SetTextForeColour(RGB(10, 109, 232));
	    //progress bar�� ���ؼ� ������ ���� ���� Text �� ����  
	    m_pgDownload.SetTextBkColour(RGB(225, 225, 225));   
	
	    //Text ���� 
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

    // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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
    strMainText.Format( "���� �ٿ�ε� ��... ... ... " );
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
        strMainText.Format( "���� �ٿ�ε� ��... ... ... " );
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
    strMainText.Format( "����̹� ������Ʈ ��... ... ... " );
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
    // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
    // �׸��� �޽����� ���ؼ��� CDialogEx::OnNcPaint()��(��) ȣ������ ���ʽÿ�.
	// �׵θ� �׸��� : http://tansanc.tistory.com/422
    CDC* pDC = GetWindowDC( ); //work out the coordinates of the window rectangle,
    CRect rect; GetWindowRect( &rect); 
    rect.OffsetRect( -rect.left, -rect.top); //Draw a single line around the outside
    CBrush brush( RGB( 10, 109, 232));
    pDC->FrameRect( &rect, &brush); 
    ReleaseDC( pDC);
}
