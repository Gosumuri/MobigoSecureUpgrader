////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MobigoSecureUpgrader.h"
#include "DebugListCtrl.h"
#include "debuglistctrl.h"
#include "MainFrm.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDebugListCtrl, CMyListCtrl)

BEGIN_MESSAGE_MAP(CDebugListCtrl, CMyListCtrl)
	ON_WM_MEASUREITEM_REFLECT( )
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CDebugListCtrl::CDebugListCtrl()
{
}

CDebugListCtrl::~CDebugListCtrl()
{
}


////////////////////////////////////////////////////////////////////////////////
// life cycle
////////////////////////////////////////////////////////////////////////////////

void CDebugListCtrl::Init(void)
{
	// 0. WM_WINDOWPOSCHANGED를 일부러 발생시켜서 부모 윈도우가 WM_MEASUREITEM 메세지를 보내도록 한다.
	CRect rc;
	GetWindowRect( &rc );

	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rc.Width();
	wp.cy = rc.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage( WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp );

	// 1. Column 크기 
	{
		// 1.1. No
		this->InsertColumn( COLUMN_INDEX_NO,	"No",			LVCFMT_LEFT,   COLUMN_WIDTH_NO		);
		// 1.2. Line
		this->InsertColumn( COLUMN_INDEX_LINE,  "Line",			LVCFMT_LEFT,   COLUMN_WIDTH_LINE	);
		// 1.3. Desc
		int nDescWidth = rc.Width() -  ( COLUMN_WIDTH_NO + COLUMN_WIDTH_LINE );
		if( nDescWidth < COLUMN_WIDTH_DESC )
			nDescWidth = COLUMN_WIDTH_DESC;
		nDescWidth -= 20; // 20 is vscrollbar's width
		this->InsertColumn( COLUMN_INDEX_DESC,  "Description",	LVCFMT_LEFT,   nDescWidth			);
	}
	// 2. style
	this->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	// 3. enable tooltip
	this->EnableToolTips(TRUE);
	this->SetRedraw(TRUE);
}

void CDebugListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 20;
}

////////////////////////////////////////////////////////////////////////////////
// event handler
////////////////////////////////////////////////////////////////////////////////

void CDebugListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CMyListCtrl::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	// 1. Column 크기 
	{
		CRect rc;
		GetWindowRect( &rc );
		// 1.1. No
		//this->SetColumnWidth( COLUMN_INDEX_NO,		COLUMN_WIDTH_NO		);
		// 1.2. Line
		//this->SetColumnWidth( COLUMN_INDEX_LINE,	COLUMN_WIDTH_LINE		);
		// 1.3. Desc
		int nDescWidth = rc.Width() -  ( COLUMN_WIDTH_NO + COLUMN_WIDTH_LINE );
		if( nDescWidth < COLUMN_WIDTH_DESC )
			nDescWidth = COLUMN_WIDTH_DESC;
		nDescWidth -= 20; // 20 is vscrollbar's width
		this->SetColumnWidth( COLUMN_INDEX_DESC,	nDescWidth				);
	}
}

void CDebugListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CMyListCtrl::OnLButtonDblClk(nFlags, point);

	int nRow = this->HitTest( point );

	if( 0 <= nRow && nRow < this->GetItemCount() )
	{
		CString				strLine		= this->GetItemText( nRow, COLUMN_INDEX_LINE );
		int					nLine		= CUtil2::Str2Int( strLine );

		CMainFrame			*pWndMain	= (CMainFrame*)AfxGetApp()->GetMainWnd();
		/*
			CGoldRushEditView    *pView		= (CGoldRushEditView*)pWndMain->GetActiveFrame()->GetActiveView();
			pView->GoToLine( nLine, false );
		*/
	}
}

////////////////////////////////////////////////////////////////////////////////
// public 
////////////////////////////////////////////////////////////////////////////////

int	CDebugListCtrl::AddRow( int nLine, CString strDesc )
{
	// 1. No
	int nNo = this->GetItemCount();
	CString strNo;
	strNo.Format( "%d", nNo + 1 );
	this->InsertItem(  nNo,	strNo );	
	// 2. Line
	if( 0 < nLine )
	{
		CString strLine = CUtil::PowerChar( '0',
											CUtil2::Int2Str( nLine ),
											6 );
		this->SetItemText( nNo, COLUMN_INDEX_LINE, strLine );
	}
	else 
	{
		this->SetItemText( nNo, COLUMN_INDEX_LINE, "" );
	}
	// 3. Desc
	this->SetItemText( nNo, COLUMN_INDEX_DESC, strDesc );
	
	// 99. return 
	return nNo;
}

