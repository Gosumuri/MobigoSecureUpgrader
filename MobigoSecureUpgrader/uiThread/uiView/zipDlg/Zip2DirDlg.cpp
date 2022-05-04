
////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MobigoSecureUpgrader.h"
#include "UiDebugView.h"
#include "UiReleaseView.h"
#include "Zip2DirDlg.h"
#include "utilzip.h"
#include "Util2.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////
// static
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CZip2DirDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
    ON_WM_WINDOWPOSCHANGING()
	ON_MESSAGE( MY_MSG_PRORGESS_ZIP_INIT,	OnMyMsgProgressZipInitHandler )  // 삽입된 부분 
	ON_MESSAGE( MY_MSG_PRORGESS_ZIPPING,	OnMyMsgProgressZippingHandler )  // 삽입된 부분 
END_MESSAGE_MAP()


////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CZip2DirDlg::CZip2DirDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZip2DirDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nFileTh		= 0;
	m_nZipTh        = 0; 

    m_pParentView   = NULL;
}

////////////////////////////////////////////////////////////////////////////////
// init funciton
////////////////////////////////////////////////////////////////////////////////

void CZip2DirDlg::InitCtrl()
{
	m_sttZipFileName.SetWindowText( "" );
	m_sttFileTh.SetWindowText( "0" );
	m_sttFileTotal.SetWindowText( "0" );
	m_pg.SetRange( 0, 100 );
	m_pg.SetPos( 0 );
	
	m_sttZipTotal.SetWindowText( "0" );
	m_sttZipTh.SetWindowText( "0" );
	m_pgTotal.SetRange( 0, 100 );
	m_pgTotal.SetPos( 0 );

	m_edLog.SetWindowText( "" );
}

void CZip2DirDlg::InitEnv()
{
}

////////////////////////////////////////////////////////////////////////////////
// virtual 
////////////////////////////////////////////////////////////////////////////////

void CZip2DirDlg::DoDataExchange(CDataExchange* pDX)
{	
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STT_ZIP_FILE_NAME,		m_sttZipFileName	);
	DDX_Control(pDX, IDC_PG,					m_pg				);
	DDX_Control(pDX, IDC_STT_FILE_TH,			m_sttFileTh			);
	DDX_Control(pDX, IDC_STT_FILE_TOTAL,		m_sttFileTotal		);

	DDX_Control(pDX, IDC_PG_TOTAL,				m_pgTotal			);
	DDX_Control(pDX, IDC_STT_ZIP_TOTAL,			m_sttZipTotal		);
	DDX_Control(pDX, IDC_STT_ZIP_TH,			m_sttZipTh			);
	DDX_Control(pDX, IDC_ED_LOG,				m_edLog				);
}

BOOL CZip2DirDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	InitCtrl();
	InitEnv();
	
	// start unzip
	SetTimer( MY_TIMER_UNZIPPING_START, 500, NULL );

    CUtilZip::SetStopUnzip( FALSE );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CZip2DirDlg::DestroyWindow()
{
    //TODO
    CUtilZip::SetStopUnzip( TRUE );

    return CDialog::DestroyWindow();
}

////////////////////////////////////////////////////////////////////////////////
// event handler 
////////////////////////////////////////////////////////////////////////////////

LRESULT CZip2DirDlg::OnMyMsgProgressZipInitHandler( WPARAM wParam, LPARAM lParam ) 
{ 
	CString *strMsg = (CString*)lParam;

	int nFileCount = atoi( *strMsg );

	m_pg.SetRange( 0, nFileCount );
	m_pg.SetStep( 1 );
	m_pg.SetPos( 0 );

	m_nFileTh = 0;
	m_sttFileTh.SetWindowText( CUtil2::Int2Str( m_nFileTh ) );
	m_sttFileTotal.SetWindowText( *strMsg );

	return TRUE;
} 

LRESULT CZip2DirDlg::OnMyMsgProgressZippingHandler( WPARAM wParam, LPARAM lParam ) 
{ 
	CString *strMsg = (CString*)lParam;
	
	m_nFileTh++;

	// 진행바의 범위를 넘지 않기 위해
	int nLower; 
	int nUpper;
	m_pg.GetRange( nLower, nUpper );
	if( m_pg.GetPos() < nUpper ) {
		m_pg.StepIt();
    }

    static int i = 0;
    i++;
    if( i % 100 == 0 ) {
	    m_sttFileTh.SetWindowText( CUtil2::Int2Str( m_nFileTh ) );
	    DEBUG_LOG( "++     " + m_ayDirPath.ElementAt( m_nZipTh ) + *strMsg );

	    // log
	    static char szLog[100];
	    sprintf( szLog, "    [%d] %s Done...!!!", m_nFileTh, (char*)(LPCTSTR)(*strMsg)  );
	    ::SendMessage( m_edLog.m_hWnd, MY_MSG_DEBUG, 0, (LPARAM)szLog );
	    //-DEBUG_LOG( "++     %s Done...!!!", (char*)(LPCTSTR)(*strMsg)  );

        // send parent 
        if( m_pParentView != NULL ) {
#ifdef FEATURE_RELEASE_VIEW 
            ((CUiReleaseView*)m_pParentView)->GetSplash()->SetSubText( szLog );
            ((CUiReleaseView*)m_pParentView)->GetSplash()->StepIt();
#else
            ((CUiDebugView*)m_pParentView)->GetSplash()->SetSubText( szLog );
            ((CUiDebugView*)m_pParentView)->GetSplash()->StepUnzip( nLower, nUpper, m_nFileTh );
#endif
        }
    }

	return TRUE;
} 

void CZip2DirDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	
	switch( nIDEvent ) {
		case MY_TIMER_UNZIPPING_START : {
			KillTimer( MY_TIMER_UNZIPPING_START );
			
			CWinThread* pWorkerThread;
			pWorkerThread = AfxBeginThread ( ThreadUnzipping, 
											 (LPVOID)this,
											 THREAD_PRIORITY_NORMAL, 
											 0, 
											 CREATE_SUSPENDED );
			if ( NULL != pWorkerThread ) {
				pWorkerThread->ResumeThread();
			}
			else {
				AfxMessageBox ( _T("Couldn't create worker thread!"), MB_ICONERROR );
			}
		}
		break;
		case MY_TIMER_UNZIPPING_END : {
			KillTimer( MY_TIMER_UNZIPPING_END );
			OnOK();
		}
		break;
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CZip2DirDlg::OnWindowPosChanging( WINDOWPOS* lpwndpos )
{
    CDialog::OnWindowPosChanging( lpwndpos );

    // TODO: 여기에 메시지 처리기 코드를 추가합니다.

#ifndef _DEBUG 
    // Dialog를 hide 시킨다. 
    if( lpwndpos != NULL ) {
        lpwndpos->flags &= ~SWP_SHOWWINDOW;
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////
// event handler 
////////////////////////////////////////////////////////////////////////////////

void CZip2DirDlg::AddZip2Dir( CString strZipPath, CString strDirPath )
{
	// normalize
	strZipPath.Replace( '/', '\\' );
	strDirPath.Replace( '/', '\\' );

	m_ayZipPath.Add( strZipPath );
	m_ayDirPath.Add( strDirPath );
}

////////////////////////////////////////////////////////////////////////////////
// unzipping Thread
////////////////////////////////////////////////////////////////////////////////

UINT CZip2DirDlg::ThreadUnzipping( LPVOID pv )
{
	CZip2DirDlg *pDlgZip2Dir = (CZip2DirDlg*)pv;

	int nCount = pDlgZip2Dir->m_ayZipPath.GetCount();

	// 1. set total progress range
	pDlgZip2Dir->m_pgTotal.SetRange( 0, nCount );
	pDlgZip2Dir->m_pgTotal.SetStep( 1 );

    pDlgZip2Dir->m_sttZipTotal.SetWindowText( CUtil2::Int2Str(nCount) );

	// 2. unziping 
	for( int i = 0; i < nCount; i++ ) {

        CString strZipPath = pDlgZip2Dir->m_ayZipPath.ElementAt( i );
        CString strDirPath = pDlgZip2Dir->m_ayDirPath.ElementAt( i );
        CString strZipFile = CUtilFile::ExtractFileName( strZipPath );

        // 2.1. log edit
	    static char szLog[100];
	    sprintf( szLog, "[%d] %s Done...!!!", i, (char*)(LPCTSTR)(strZipFile)  );
	    ::SendMessage( pDlgZip2Dir->m_edLog.m_hWnd, MY_MSG_DEBUG, 0, (LPARAM)szLog );

		// 2.2. th
		pDlgZip2Dir->m_sttZipTh.SetWindowText( CUtil2::Int2Str( i ) );
		pDlgZip2Dir->m_nZipTh = i;
		// 2.3. zip file name
		pDlgZip2Dir->m_sttZipFileName.SetWindowText( strZipFile );
		// 2.4. unzipping
		CUtilZip::Zip2Dir(  pDlgZip2Dir->m_ayZipPath.ElementAt( i ),
							pDlgZip2Dir->m_ayDirPath.ElementAt( i ),
							pDlgZip2Dir->m_hWnd ); 

        // 2.5. unzip complete
        static char szZipPath[512];    
        memset( szZipPath, 0, sizeof(szZipPath)/sizeof(char));
        strncpy( szZipPath, strZipPath.GetString(), sizeof(szZipPath)/sizeof(char) - 1 );
		DEBUG_LOG( "++   szZipPath=%s", szZipPath );
        SEND_UI_THREAD( MY_MSG_U_COMPLETE_UNZIP_EACH_ITEM, (WPARAM)&szZipPath, (LPARAM)NULL );

        // 2.6. stop unzip
        if( CUtilZip::IsStopUnzip() == TRUE ) {
	        return TRUE;
        }

		// 2.7. step it
		pDlgZip2Dir->m_pgTotal.StepIt();
	}

	// 3. 마무리
	pDlgZip2Dir->m_sttZipTh.SetWindowText( CUtil2::Int2Str( nCount ) );
	pDlgZip2Dir->m_pgTotal.SetPos( nCount );

	pDlgZip2Dir->SetTimer( MY_TIMER_UNZIPPING_END, 500, NULL );

	return TRUE;
}