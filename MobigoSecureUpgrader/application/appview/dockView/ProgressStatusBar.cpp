// ProgressStatusBar.cpp : implementation file
//

#include "stdafx.h"
#include "ProgressStatusBar.h"
#include "resource.h"
#include "debug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MY_TIMER_END		1
#define MY_TIMER_LAPTIME	2

////////////////////////////////////////////////////////////////////////////////////////////////////
// CStatusBar

CProgressStatusBar::CProgressStatusBar()
{
	m_bShow				= FALSE;
	m_pgrStatus.SetStyle(PROGRESS_TEXT);
	m_bConnected		= FALSE;
	m_dwLapTimeStart	= 0;
}

CProgressStatusBar::~CProgressStatusBar()
{
}

BEGIN_MESSAGE_MAP(CProgressStatusBar, CStatusBar)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_MESSAGE(MY_MSG_PROGRESS_START,      OnProgressStart   )
	ON_MESSAGE(MY_MSG_PROGRESS_STEP,       OnProgressStep    )
	ON_MESSAGE(MY_MSG_PROGRESS_END,        OnProgressEnd     )
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// timer handler
////////////////////////////////////////////////////////////////////////////////

void CProgressStatusBar::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	switch( nIDEvent ) {
		case MY_TIMER_END : {
			KillTimer( MY_TIMER_END );

			if( m_bShow ) {
				m_bShow = FALSE;

				// 1. reset
				m_pgrStatus.SetPos( 0 );
				m_pgrStatus.SetText( "" );

				// 2. progress bar�� �ı��Ѵ�.
				m_pgrStatus.ShowWindow( SW_HIDE );
				m_pgrStatus.DestroyWindow();
				
				// 3. status bar�� text�� �Ѹ���. 
				SetPaneText( 0, "Ready");
			}
		}
		break;
		case MY_TIMER_LAPTIME : {
			if( m_bShow ) {
				Step( -1 );
			}
		}
		break;
	}

	CStatusBar::OnTimer(nIDEvent);
}

////////////////////////////////////////////////////////////////////////////////
// Device Monitor Listener ( event handler )
////////////////////////////////////////////////////////////////////////////////

LRESULT CProgressStatusBar::OnProgressStart(WPARAM wParam, LPARAM lParam)
{
    Start( CString(((char*)wParam)), (int)lParam ) ; 
    SetConnected( TRUE );
	return TRUE;
}
LRESULT CProgressStatusBar::OnProgressStep(WPARAM wParam, LPARAM lParam)
{
    Step( (int)lParam ) ; 
	return TRUE;
}
LRESULT CProgressStatusBar::OnProgressEnd(WPARAM wParam, LPARAM lParam)
{
    End() ; 
    SetConnected( FALSE );
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// operator

void CProgressStatusBar::Start(	CString strLabel, int nTotal )  
{
	if( m_bShow == FALSE ) {
		m_bShow = TRUE;

		CRect rect;
		GetItemRect(0, rect);
		// 1. progress bar�� ������ ��ġ. 
		rect.left = 100;
	
		// progress bar�� �����Ѵ�.
		if( !m_pgrStatus.Create( WS_CHILD|WS_VISIBLE, 
								rect, 
								this, 
								AFX_IDW_TOOLBAR + 100 )) {
			AfxMessageBox("fail to create progress");
			return;
		}

		// 2. status bar�� text�� �Ѹ���. 
		SetPaneText( 0, strLabel );
		UpdateWindow();

		// 3. progress bar�� ������ ����. 
		m_pgrStatus.SetRange(0, nTotal);

		// 4. progress bar�� �ʱ� ��ġ ����. 
		m_pgrStatus.SetPos(  0 );
		m_pgrStatus.SetStep( 1 );

		// 5. �ణ�� ������¸� ǥ���Ѵ�
		// m_pgrStatus.StepIt();
		m_pgrStatus.SetText( "0.00 %      ( 0.0 sec )" );

		// 6. laptime
		SetTimer( MY_TIMER_LAPTIME, 300, NULL ); 
		m_dwLapTimeStart = ::GetTickCount();
	}
}
void CProgressStatusBar::Step(int nStep )
{
	if( m_bShow ) {
		int nCurStep = m_pgrStatus.GetPos();

		// 1. Step �� �ð��� �����Ų��.
		if( nCurStep < nStep || nStep == -1  ) {
			// 1.1. pos
			if( nStep != -1 ) {
				m_pgrStatus.SetPos( nStep );
			}
			int   nPos   = m_pgrStatus.GetPos();
			int   nLower, nUpper;
			m_pgrStatus.GetRange( nLower, nUpper );

			// 1.2. lap time
			DWORD dwLapTime = ::GetTickCount() - m_dwLapTimeStart; 

			// 1.3. text
			float fPos = (float)nPos/(float)nUpper;
			CString strText;
			strText.Format( "%4.2f %%      ( %4.1f sec )", fPos*100, ((float)dwLapTime/(float)1000) );
			m_pgrStatus.SetText( strText );
		}
	}
}

void CProgressStatusBar::End()
{
	if( m_bShow == TRUE ) {

		// 1. pos
		int   nLower, nUpper;
		m_pgrStatus.GetRange( nLower, nUpper );
		m_pgrStatus.SetPos(   nUpper );

		// 2. lap time
		DWORD dwLapTime = ::GetTickCount() - m_dwLapTimeStart; 

		// 3. text
		CString strText;
		strText.Format( "100.00 %%      ( %3.1f sec )", ((float)dwLapTime/(float)1000) );
		m_pgrStatus.SetText( strText );
		SetTimer( MY_TIMER_END, 1*1000, NULL );

		// 4. kill timer
		KillTimer( MY_TIMER_LAPTIME );
		
		// 5. lap time
		strText.Format( "\t%3.1f sec\r\n", ((float)dwLapTime/(float)1000) );
		DEBUG_LOG("++   Lap Time = " + strText );
	}
}
void CProgressStatusBar::OnPaint()
{
	// CPaintDC dc(this); 
    // ���� CPaintDC�� ���ԵǸ� �ɰ��� ������ �߻��Ѵ�.
	// �� ������°� ���¹ٿ� �ٸ� �͵��� �������� �ʴ´ٴ� ���ε�..
	// �ؼ� �ؿ�ó�� Ŭ���̾�ƮDC�� ��߸� �Ѵ�.
		
	CDC* pdc = GetWindowDC();
	// �����ִ� ��ɾ�.
	UpdateAllPanes(TRUE, TRUE);
    // ù��° ���ڰ� TRUE�̸� ���¹��� ��� ���ε��� ������ �ٽ� �׸��鼭 
	// �װ͵��� ������ ���κк��� ���Ľ�Ų��. ���� ���������� ����ũ�⸦ 
	// ������ �� ������ ���ο� �Ҵ�� ����ũ�⺸�� ���� ������ ������ ��
	// Ŀ���� �Ǹ� SBPS_STRETCH�� �����Ǿ� �ִ� ������ ������ ũ�⸸ŭ �� Ŀ���� �ȴ�.
	// �׸��� �ι�° ���ڰ� TRUE�̸� ���¹ٿ� ������ �ٽ� �׸���.
  	Default();

	//
	CRect rect1;
	CBitmap bitmap1;
	CString strMsg;	

	// 1. ���¿� �´� ��Ʈ�� �ڿ��� String���� �����Ѵ�. 
	if( m_bConnected == TRUE ) {
		bitmap1.LoadBitmap(IDB_GREEN_BALL);
		strMsg = "Running";
	} else {
		bitmap1.LoadBitmap(IDB_GRAY_BALL);
		strMsg = "Ready";
	}

	// 2. image�� �׸���.
	{ 
		// "ǥ�õ�"�� ��Ÿ���� ������ ũ��� ��ġ�� ���մϴ�.
		GetItemRect(1, rect1);
		// ������ 3���� �׵θ��� �� �簢���� �����մϴ�.
		rect1.InflateRect(-1, -1);
		// �������� ���õ� �κ��� �����ϰ� ���¹ٸ� ĥ�մϴ�.
		pdc->ExcludeClipRect(&rect1);
		// ��ġ ���ؽ�Ʈ�� ���� Ŭ���ο����� ���¹��� ������ 
		// �簢���� �ǵ���  �����մϴ�.
		CRgn paneRgn1;
		paneRgn1.CreateRectRgnIndirect(rect1);
		pdc->SelectClipRgn(&paneRgn1);
		paneRgn1.DeleteObject();
		// ��Ʈ���� �������� �����մϴ�.
		CDC srcDC1;
		srcDC1.CreateCompatibleDC(pdc);
		CBitmap* pOldBitmap1 = srcDC1.SelectObject(&bitmap1);
		
		// Bitmap file�� �׸���.
		pdc->BitBlt( rect1.left,
					 rect1.top,
					 rect1.Width(), 
					 rect1.Height(), 
					 &srcDC1, 
					 0, 
					 0, 
					 SRCCOPY); 

		srcDC1.SelectObject(pOldBitmap1);
	}

	// 3. String�� �׸���.
	{
		CFont font;
		font.CreateFont( 15,0,0,0,NULL,0,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS, NULL ); // "Arial" );
		CFont* pOldFont = pdc->SelectObject(&font);

		pdc->SetBkMode(TRANSPARENT);
		pdc->SetTextColor( RGB (0,0,0) );
		DWORD dwStyle = DT_LEFT | DT_SINGLELINE;
		pdc->DrawText(	strMsg, 
						CRect( rect1.left + 18,
							   rect1.top,
							   rect1.left + 100,
							   rect1.bottom ), 
						dwStyle | DT_SINGLELINE );


		// font restore		
		pdc->SelectObject(pOldFont);
	}
}
