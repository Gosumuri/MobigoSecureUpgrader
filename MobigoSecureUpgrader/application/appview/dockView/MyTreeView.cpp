// MyTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "MyTreeView.h"
#include "resource.h"

// CMyTreeView
IMPLEMENT_DYNCREATE(CMyTreeView, CTreeView)

CMyTreeView::CMyTreeView()
{
}

CMyTreeView::~CMyTreeView()
{
}

BEGIN_MESSAGE_MAP(CMyTreeView, CTreeView)
END_MESSAGE_MAP()


// CMyTreeView diagnostics

#ifdef _DEBUG
void CMyTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CMyTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

////////////////////////////////////////////////////////////////////////////////////////////////////
// image

void CMyTreeView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	
	InitIconList();
}

BOOL CMyTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class

	cs.style	|= TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS /*|TVS_EDITLABELS*/ ;
	cs.style	&= ~TVS_SHOWSELALWAYS;

	return CTreeView::PreCreateWindow(cs);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// image

void CMyTreeView::InitIconList()
{
	m_imgList.Create( 16, 16, ILC_COLOR16, 10, 5 );

	AddIcon(IDB_MOBIGO);
	AddIcon(IDB_BUG);
	AddIcon(IDB_CHECKBOX_GRAY);
	AddIcon(IDB_CHECKBOX_OFF);
	AddIcon(IDB_CHECKBOX_ON);
	AddIcon(IDB_CLEAR);
	AddIcon(IDB_DICE_1);
	AddIcon(IDB_DICE_2);
	AddIcon(IDB_DICE_3);
	AddIcon(IDB_DICE_4);
	AddIcon(IDB_DICE_5);
	AddIcon(IDB_DICE_6);
	AddIcon(IDB_FOLDER);
	AddIcon(IDB_GREEN_BALL);
	AddIcon(IDB_GRAY_BALL);
	AddIcon(IDB_LG);
	AddIcon(IDB_PATTERN);
	AddIcon(IDB_SAMSUNG);
	AddIcon(IDB_SETTING);
	AddIcon(IDB_STOP);
	AddIcon(IDB_TEST_TUBE);
	AddIcon(IDB_TRANSFER);
	AddIcon(IDB_HTM);
	AddIcon(IDB_JAVA);
	AddIcon(IDB_FEATURE_PHONE);
	AddIcon(IDB_SMART_PHONE);
	AddIcon(IDB_MESSAGEBOX);
	AddIcon(IDB_BUILD);
    AddIcon(IDB_LOG);
    AddIcon(IDB_LOG_1);
	AddIcon(IDB_UPGRADE);
	AddIcon(IDB_WATCH);
	AddIcon(IDB_SAVE);
	AddIcon(IDB_PORT);
	AddIcon(IDB_USB);
	AddIcon(IDB_MOUSE);
	AddIcon(IDB_MONITOR);
	AddIcon(IDB_WOWMAN);
	AddIcon(IDB_SEARCH);
	AddIcon(IDB_MAN);
	AddIcon(IDB_PERCENT);
	AddIcon(IDB_PLUS);
	AddIcon(IDB_MINUS);
	AddIcon(IDB_UNKNOWN);

	GetTreeCtrl().SetImageList(&m_imgList, TVSIL_NORMAL);

	m_imgList.Detach();
}

void CMyTreeView::AddIcon( int nBitmapID )
{
	CBitmap	bitmap;
	bitmap.LoadBitmap( nBitmapID );
	m_imgList.Add(&bitmap, RGB(0, 0, 0));
	bitmap.DeleteObject();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Util Functions

void CMyTreeView::ExpandChildItem( int nDepth, HTREEITEM hItem)
{
	CTreeCtrl& treeCtrl = this->GetTreeCtrl();

	if ( hItem == NULL )
		hItem = treeCtrl.GetRootItem();
	if ( nDepth == 0 )
		return;
	else {
		treeCtrl.Expand(hItem, TVE_EXPAND);

		HTREEITEM htiChild = treeCtrl.GetChildItem( hItem );
		// Scan children
		while( htiChild ) {
			ExpandChildItem(nDepth-1, htiChild);
			htiChild = treeCtrl.GetNextItem( htiChild, TVGN_NEXT );
		}
	}
}
