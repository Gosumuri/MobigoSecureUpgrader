////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConfigTreeView.h"
#include "utilxml.h"

#include "MobigoSecureUpgrader.h"


////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CConfigTreeView, CXMLTreeView)

BEGIN_MESSAGE_MAP(CConfigTreeView, CXMLTreeView)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_DBLCLK,			OnNMDblclk				)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CConfigTreeView::CConfigTreeView()
{
	m_bClickIcon = FALSE;
}

CConfigTreeView::~CConfigTreeView()
{
}

////////////////////////////////////////////////////////////////////////////////
// debug & diagnostics
////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void CConfigTreeView::AssertValid() const
{
	CXMLTreeView::AssertValid();
}

void CConfigTreeView::Dump(CDumpContext& dc) const
{
	CXMLTreeView::Dump(dc);
}
#endif //_DEBUG

////////////////////////////////////////////////////////////////////////////////
// event handler
////////////////////////////////////////////////////////////////////////////////

void CConfigTreeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	
	// 1. Double Click을 위한 준비
	m_ptLButton		= point;

	if( m_bClickIcon == TRUE ) { 
		return; 
	}

	HTREEITEM	hItem;
	CTreeCtrl& treeCtrl = this->GetTreeCtrl();
		
	// 1. 해당 위치의 아이템을 구해서, 그 아이템이 선택되도록
	hItem				= treeCtrl.HitTest(m_ptLButton); 
	treeCtrl.SelectItem(hItem);					 

	// 2. 바닥을 찍었으면 그냥 보내고
	if(hItem == NULL) 
		return;

	// 3. item이 Leaf 인지 확인
#if 0 // single click으로 수행되는 것으로 방지
	if( treeCtrl.GetChildItem(hItem) == NULL ) {
		CView *pView = ((CMobigoSecureApp*)AfxGetApp())->GetActiveView();
		if( pView != NULL ) {
			IXMLDOMElement*		pEle = (IXMLDOMElement*)GetTreeCtrl().GetItemData( hItem );
			pView->SendMessage( MY_MSG_OPEN_VIEW, (WPARAM)pEle, (LPARAM)NULL );
		}
	}
#endif

	CXMLTreeView::OnLButtonDown(nFlags, point); 
}

void CConfigTreeView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	DEBUG_LOG(">> CConfigTreeView::OnNMDblclk()");

	if( m_bClickIcon == TRUE ) { 
		*pResult = TRUE;
		return; 
	}

	HTREEITEM	hItem;
	CTreeCtrl& treeCtrl = this->GetTreeCtrl();
		
	// 1. 해당 위치의 아이템을 구해서, 그 아이템이 선택되도록
	hItem				= treeCtrl.HitTest(m_ptLButton); 
	treeCtrl.SelectItem(hItem);					 

	// 2. 바닥을 찍었으면 그냥 보내고
	if(hItem == NULL) 
		return;

	// 3. item이 Leaf 인지 확인
    //  - Leaf 노트가 아니여도, 실행 되도록 한다( 20170919_jkim )
	if( TRUE || treeCtrl.GetChildItem(hItem) == NULL ) {
		CView *pView = ((CMobigoSecureUpgraderApp*)AfxGetApp())->GetActiveView();
		if( pView != NULL ) {
			IXMLDOMElement*		pEle = (IXMLDOMElement*)GetTreeCtrl().GetItemData( hItem );
			CString strID = CUtilXML::GetID(pEle);
			if (strID.IsEmpty() == FALSE) {             
				DEBUG_LOG("++   strID=" + strID );

                // Zoom
                if (strID == "ID_FUNC_ZOOM_OUT")       POST_UI_THREAD( MY_MSG_U_TDD, (WPARAM)"ID_FUNC_ZOOM_OUT",    (LPARAM)NULL);
                if (strID == "ID_FUNC_ZOOM_IN")        POST_UI_THREAD( MY_MSG_U_TDD, (WPARAM)"ID_FUNC_ZOOM_IN",     (LPARAM)NULL);

                // Save Config
                if (strID == "ID_FUNC_SAVE_CONFIG")    AfxGetMainWnd()->SendMessage( WM_COMMAND, ID_SAVE_CONFIG );
			}
		}
	}

	*pResult = 0;
}


////////////////////////////////////////////////////////////////////////////////
// check on/off
////////////////////////////////////////////////////////////////////////////////

BOOL CConfigTreeView::IsAllCheckOn( HTREEITEM hItem )
{
/*
    HTREEITEM	hChild = GetTreeCtrl().GetNextItem( hItem, TVGN_CHILD);

    while( hChild ) {
		
		// 1. image index
		int nImage			= -1;
		int nSelectedImage	= -1;
		GetTreeCtrl().GetItemImage( hChild, nImage, nSelectedImage );		

		// 2. check_off 인지 검사
		if( nImage == TREE_IMAGE_CHECKBOX_OFF ) {
			return FALSE;
		}

		// 3. horizontal search 
        hChild = GetTreeCtrl().GetNextItem( hChild, TVGN_NEXT );
    }
*/  
	return TRUE;
}

BOOL CConfigTreeView::IsAllCheckOff( HTREEITEM hItem )
{
/*
    HTREEITEM	hChild = GetTreeCtrl().GetNextItem( hItem, TVGN_CHILD);

    while( hChild ) {
		
		// 1. image index
		int nImage			= -1;
		int nSelectedImage	= -1;
		GetTreeCtrl().GetItemImage( hChild, nImage, nSelectedImage );		

		// 2. check_off 인지 검사
		if( nImage == TREE_IMAGE_CHECKBOX_ON ) {
			return FALSE;
		}

		// 3. horizontal search 
        hChild = GetTreeCtrl().GetNextItem( hChild, TVGN_NEXT );
    }
*/  
	return TRUE;
}

void CConfigTreeView::AllCheckOn( HTREEITEM hItem )
{
/*
   HTREEITEM	hChild = GetTreeCtrl().GetNextItem( hItem, TVGN_CHILD);

    while( hChild ) {
		
		GetTreeCtrl().SetItemImage( hChild, TREE_IMAGE_CHECKBOX_ON, TREE_IMAGE_CHECKBOX_ON );				

		// 3. horizontal search 
        hChild = GetTreeCtrl().GetNextItem( hChild, TVGN_NEXT );
    }
*/
}

void CConfigTreeView::AllCheckOff( HTREEITEM hItem )
{
/*
   HTREEITEM	hChild = GetTreeCtrl().GetNextItem( hItem, TVGN_CHILD);

    while( hChild ) {
		
		GetTreeCtrl().SetItemImage( hChild, TREE_IMAGE_CHECKBOX_OFF, TREE_IMAGE_CHECKBOX_OFF );				

		// 3. horizontal search 
        hChild = GetTreeCtrl().GetNextItem( hChild, TVGN_NEXT );
    }
*/
}
