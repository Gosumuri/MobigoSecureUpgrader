// TreeControlBar.cpp : implementation file
//

#include "stdafx.h"
#include "TreeControlBar.h"
#include "resource.h"

// CTreeControlBar
IMPLEMENT_DYNAMIC( CTreeControlBar, CSizingTabCtrlBar )

CTreeControlBar::CTreeControlBar()
{
}

CTreeControlBar::~CTreeControlBar()
{	
}

BEGIN_MESSAGE_MAP(CTreeControlBar, CSizingTabCtrlBar )
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CTreeControlBar message handlers

void CTreeControlBar::OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL /*bDisableIfNoHndler*/)
{
}

int CTreeControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CSizingTabCtrlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	
	// 1. image list
	m_images.Create(IDB_WORKSPACE, 15, 1, RGB(255,0,255));
	m_tabctrl.SetImageList(&m_images);	

	return 0;
}

void CTreeControlBar::MoveView( int cx, int cy)
{
	int bottom = (IsHorz() || IsFloating()) ? cy - 14 : cy - 18;

	m_tabctrl.MoveWindow( 7, 7, cx - 14, bottom );

	// 1. View
	CWnd *pWnd;
    for (POSITION pos=m_views.GetHeadPosition(); pos; m_views.GetNext(pos)) {
         pWnd=m_views.GetAt(pos)->pWnd;

		 // Top Margin ºÎ¿©
		 pWnd->SetWindowPos(&wndTop, 
							9, 
							9, // + TREEVIEW_TOP_MARGIN, 
							cx-19, 
							bottom - 26,  // - TREEVIEW_TOP_MARGIN, 
							NULL);
		 pWnd->RedrawWindow();

    }
}

void CTreeControlBar::OnSize(UINT nType, int cx, int cy)
{
	CSizingTabCtrlBar::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
}

