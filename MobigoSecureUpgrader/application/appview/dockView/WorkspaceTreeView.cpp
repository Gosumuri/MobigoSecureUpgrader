////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WorkspaceTreeView.h"
#include "utilxml.h"
#include "MobigoSecureUpgrader.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// macro
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CWorkspaceTreeView, CXMLTreeView)

BEGIN_MESSAGE_MAP(CWorkspaceTreeView, CXMLTreeView)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_DBLCLK,			OnNMDblclk				)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////
CWorkspaceTreeView::CWorkspaceTreeView()
{
	m_bClickIcon = FALSE;
}

CWorkspaceTreeView::~CWorkspaceTreeView()
{
}

////////////////////////////////////////////////////////////////////////////////
// diagnostics & debug
///////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void CWorkspaceTreeView::AssertValid() const
{
	CXMLTreeView::AssertValid();
}

void CWorkspaceTreeView::Dump(CDumpContext& dc) const
{
	CXMLTreeView::Dump(dc);
}
#endif //_DEBUG


////////////////////////////////////////////////////////////////////////////////
// event handler
///////////////////////////////////////////////////////////////////////////////

void CWorkspaceTreeView::OnLButtonDown(UINT nFlags, CPoint point)
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
		CView *pView = ((CMobigoSecureUpgrader*)AfxGetApp())->GetActiveView();
		if( pView != NULL ) {
			IXMLDOMElement*		pEle = (IXMLDOMElement*)GetTreeCtrl().GetItemData( hItem );
			pView->SendMessage( MY_MSG_OPEN_VIEW, (WPARAM)pEle, (LPARAM)NULL );
		}
	}
#endif

	CXMLTreeView::OnLButtonDown(nFlags, point); 
}

void CWorkspaceTreeView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	DEBUG_LOG(">> CWorkspaceTreeView::OnNMDblclk()");

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

                // tdd(basic)
				if (strID == "TDD_UPGRADE_LIST")            POST_UI_THREAD(       MY_MSG_U_TDD,  (WPARAM)"TDD_UPGRADE_LIST",        (LPARAM)NULL);
    			if (strID == "TDD_POLICY_LIST")             POST_UI_THREAD(       MY_MSG_U_TDD,  (WPARAM)"TDD_POLICY_LIST",         (LPARAM)NULL);
    			if (strID == "TDD_DOWNLOAD_DIALOG")         POST_UI_THREAD(       MY_MSG_U_TDD,  (WPARAM)"TDD_DOWNLOAD_DIALOG",     (LPARAM)NULL);
                if (strID == "TDD_UNZIP_BY_UTIL")           POST_UI_THREAD(       MY_MSG_U_TDD,  (WPARAM)"TDD_UNZIP_BY_UTIL",       (LPARAM)NULL);
                if (strID == "TDD_UNZIP_BY_DLG")            POST_UI_THREAD(       MY_MSG_U_TDD,  (WPARAM)"TDD_UNZIP_BY_DLG",        (LPARAM)NULL);

                // tdd(ui)
				if (strID == "TDD_SHOW_SPLASH")             POST_UI_THREAD(       MY_MSG_U_TDD,  (WPARAM)"TDD_SHOW_SPLASH",         (LPARAM)NULL);

                // Custom MessageBox
                if (strID == "ID_FUNC_SYSTEM_ALERT")        POST_UI_THREAD(       MY_MSG_U_TDD,  (WPARAM)"ID_FUNC_SYSTEM_ALERT",                     (LPARAM)NULL);
                if (strID == "ID_FUNC_SYSTEM_CONFIRM")      POST_UI_THREAD(       MY_MSG_U_TDD,  (WPARAM)"ID_FUNC_SYSTEM_CONFIRM",                   (LPARAM)NULL);
                if (strID == "ID_FUNC_HTML_ALERT")          POST_UI_THREAD(       MY_MSG_U_TDD,  (WPARAM)"ID_FUNC_HTML_ALERT",                       (LPARAM)NULL);
                if (strID == "ID_FUNC_HTML_CONFIRM")        POST_UI_THREAD(       MY_MSG_U_TDD,  (WPARAM)"ID_FUNC_HTML_CONFIRM",                     (LPARAM)NULL);
                if (strID == "ID_FUNC_NETWORK")             POST_UI_THREAD(       MY_MSG_U_TDD,  (WPARAM)"ID_FUNC_NETWORK",                          (LPARAM)NULL);

                // tdd(service)
				if (strID == "ID_SERVICE_INSTALL")          POST_UI_THREAD(       MY_MSG_U_TDD,  (WPARAM)"ID_SERVICE_INSTALL",      (LPARAM)NULL);
				if (strID == "ID_SERVICE_START")            POST_UI_THREAD(       MY_MSG_U_TDD,  (WPARAM)"ID_SERVICE_START",        (LPARAM)NULL);
				if (strID == "ID_SERVICE_STOP")             POST_UI_THREAD(       MY_MSG_U_TDD,  (WPARAM)"ID_SERVICE_STOP",         (LPARAM)NULL);
				if (strID == "ID_SERVICE_UNINSTALL")        POST_UI_THREAD(       MY_MSG_U_TDD,  (WPARAM)"ID_SERVICE_UNINSTALL",    (LPARAM)NULL);

                // Debug
				if (strID == "ID_DEBUG_ON_OFF") ;
				if (strID == "ID_DEBUG_CLEAR" )             DEBUG_CLEAR();
				if (strID == "ID_DEBUG_PAUSE" )             DEBUG_TOGGLE();
			}
		}
	}

	*pResult = 0;
}

////////////////////////////////////////////////////////////////////////////////
// utility
///////////////////////////////////////////////////////////////////////////////

BOOL CWorkspaceTreeView::IsAllCheckOn( HTREEITEM hItem )
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

BOOL CWorkspaceTreeView::IsAllCheckOff( HTREEITEM hItem )
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

void CWorkspaceTreeView::AllCheckOn( HTREEITEM hItem )
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

void CWorkspaceTreeView::AllCheckOff( HTREEITEM hItem )
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
