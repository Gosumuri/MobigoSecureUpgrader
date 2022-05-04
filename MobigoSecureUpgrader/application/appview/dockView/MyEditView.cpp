// MyEditView.cpp : implementation file
//

#include "stdafx.h"
#include "MyEditView.h"

// CMyEditView

IMPLEMENT_DYNCREATE(CMyEditView, CEditView)

CMyEditView::CMyEditView()
{
}

CMyEditView::~CMyEditView()
{
}

BEGIN_MESSAGE_MAP(CMyEditView, CEditView)
	//{{AFX_MSG_MAP(CTesteditView)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
	ON_MESSAGE( MY_MSG_DEBUG,   OnMsgDebugHandler   )  // debug
	ON_MESSAGE( MY_MSG_WARNING, OnMsgWarningHandler )  // warning
	ON_MESSAGE( MY_MSG_ERROR,   OnMsgErrorHandler   )  // error
	ON_MESSAGE( MY_MSG_WATCH,   OnMsgWatchHandler   )  // watch
	ON_CONTROL_REFLECT_EX(EN_CHANGE, OnEditChange)
    //	ON_WM_KEYDOWN()
    ON_WM_CHAR()
END_MESSAGE_MAP()

// CMyEditView diagnostics

#ifdef _DEBUG
void CMyEditView::AssertValid() const
{
	CEditView::AssertValid();
}

void CMyEditView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG


// CMyEditView message handlers

HBRUSH CMyEditView::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	pDC->SetBkColor(RGB(255, 255, 255));
	static CBrush br(RGB(255, 255, 255));

	return br;
}

BOOL CMyEditView::OnEditChange()
{
	// 20031024 : jkim
	//  - 편집이 이루어질 경우, 이 함수가 호출된다. 
	//	- GetDocument의 return value는 NULL이다.
	//  - 따라서 주석 처리했음.

//	ASSERT_VALID(this);
//	GetDocument()->SetModifiedFlag();
//	ASSERT_VALID(this);

	return FALSE;   // continue routing
}

void CMyEditView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
//	GetEditCtrl().SetReadOnly(TRUE);

	CClientDC dc(this);
	m_fontDefault.CreateFont(  -MulDiv(10, dc.GetDeviceCaps(LOGPIXELSX), 72), 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, _T("Courier New"));
	SetFont( &m_fontDefault );
}

/////////////////////////////////////////////////////////////////////////////
// message handlers

void CMyEditView::AddString( LPARAM lParamString )
{
	CEdit&		editCtrl = GetEditCtrl();
	CString     strMsg( (char*)lParamString );
/*
	// 방법 1 : 
	int nStartChar, nEndChar, nSize;
	editCtrl.SendMessage( EM_GETSEL, (WPARAM) &nStartChar, (LPARAM) &nEndChar );
	nSize = (int) editCtrl.SendMessage( WM_GETTEXTLENGTH, 0, 0 );
	if( nStartChar == nSize && nSize == nEndChar  ) {
		SCROLLINFO si;
		si.cbSize	= sizeof si;
		si.fMask	= SIF_ALL;
		editCtrl.GetScrollInfo( SB_VERT, &si );
		if( si.nPos >= si.nMax - si.nPage + 1  ) {
			editCtrl.SendMessage( EM_SETSEL, (WPARAM) nSize, (LPARAM) nSize );
			editCtrl.SendMessage( EM_REPLACESEL, 0, (LPARAM)lParamString );
			editCtrl.SendMessage( EM_REPLACESEL, 0, (LPARAM)"\r\n" );
		}
	}
*/

/*
	// 방법 2 : 
	CString   strBuffer; 
	editCtrl.GetWindowText (strBuffer);
	strBuffer += strMsg;
	editCtrl.SetWindowText (strBuffer);

	// Scroll the edit control
	editCtrl.LineScroll(editCtrl.GetLineCount(), 0);
*/

/*
	// 방법 3 : 
	editCtrl.SetReadOnly(FALSE);

	// 3.1. Clipboar에 저장
	if(OpenClipboard()) {
		HGLOBAL clipbuffer;
		char * buffer;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, strMsg.GetLength()+1);
		buffer = (char*)GlobalLock(clipbuffer);
		strcpy(buffer, LPCSTR(strMsg));
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT,clipbuffer);
		CloseClipboard();
	}
	
	// 3.2. caret을 마지막 위치로
	int nStartChar, nEndChar, nSize;
	editCtrl.SendMessage( EM_GETSEL, (WPARAM) &nStartChar, (LPARAM) &nEndChar );
	nSize = (int) editCtrl.SendMessage( WM_GETTEXTLENGTH, 0, 0 );
	if( nStartChar == nSize && nSize == nEndChar  ) {
		SCROLLINFO si;
		si.cbSize	= sizeof si;
		si.fMask	= SIF_ALL;
		editCtrl.GetScrollInfo( SB_VERT, &si );
		if( si.nPos >= si.nMax - si.nPage + 1  ) 
			editCtrl.SendMessage( EM_SETSEL, (WPARAM) nSize, (LPARAM) nSize );
	}

	// 3.3. 붙이기
	editCtrl.Paste();

	editCtrl.SetReadOnly(TRUE);
*/
	// 방법 4
	editCtrl.SetSel( INT_MAX, -1 );
	editCtrl.ReplaceSel( strMsg );
}

// debug
LRESULT CMyEditView::OnMsgDebugHandler( WPARAM wParam, LPARAM lParam ) 
{ 
	if( ::IsWindowVisible( this->m_hWnd ) )
		AddString( lParam );

	return TRUE;
} 

// warning
LRESULT CMyEditView::OnMsgWarningHandler( WPARAM wParam, LPARAM lParam ) 
{ 
	if( ::IsWindowVisible( this->m_hWnd ) )
		AddString( lParam );

	return TRUE;
} 

// error
LRESULT CMyEditView::OnMsgErrorHandler( WPARAM wParam, LPARAM lParam ) 
{ 
	if( ::IsWindowVisible( this->m_hWnd ) )
		AddString( lParam );

	return TRUE;
} 

// watch
LRESULT CMyEditView::OnMsgWatchHandler( WPARAM wParam, LPARAM lParam ) 
{ 
	if( ::IsWindowVisible( this->m_hWnd ) )
		AddString( lParam );

	return TRUE;
} 

//BOOL CMyEditView::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: Add your specialized code here and/or call the base class
// 
//	if(pMsg->message == WM_KEYDOWN ) {
//		if(		pMsg->wParam == VK_LEFT ) {
//			m_wmlView.InputKey( CWmlView::EMULATOR_KEY_LEFT );
//			return TRUE;
//		}
//		else if( pMsg->wParam == VK_UP ) {
//			m_wmlView.InputKey( CWmlView::EMULATOR_KEY_UP );
//			return TRUE;
//		}
//		else if( pMsg->wParam == VK_RIGHT ) {
//			m_wmlView.InputKey( CWmlView::EMULATOR_KEY_RIGHT );
//			return TRUE;
//		}
//		else if( pMsg->wParam == VK_DOWN ) {
//			m_wmlView.InputKey( CWmlView::EMULATOR_KEY_DOWN );
//			return TRUE;
//		}
//		else if( pMsg->wParam == VK_RETURN ) {
//			m_edInput.GetWindowText(strTemp);
//			if ( strTemp != "" )  {
//				m_wmlView.InputCommand((LPCTSTR)strTemp);
//			}
//			else {
//				m_wmlView.InputKey(CWmlView::EMULATOR_KEY_ENTER);
//			}
//			return TRUE;
//		}
//	}
//
//	return CEditView::PreTranslateMessage(pMsg);
//}

void CMyEditView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CEditView::OnChar(nChar, nRepCnt, nFlags);
}
