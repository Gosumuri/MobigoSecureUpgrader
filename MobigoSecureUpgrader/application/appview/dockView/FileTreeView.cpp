// FileTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "MobigoSecureUpgrader.h"
#include "FileTreeView.h"
#include "utilxml.h"
#include "Debug.h"

#define IDM_POPEXPLORER     20000

// CFileTreeView

IMPLEMENT_DYNCREATE(CFileTreeView, CMyTreeView)

CFileTreeView::CFileTreeView()
{
    m_bClickIcon = FALSE;
}

CFileTreeView::~CFileTreeView()
{
    m_ContextMenu.DestroyMenu();
}

BEGIN_MESSAGE_MAP(CFileTreeView, CMyTreeView)
    ON_WM_LBUTTONDOWN()
    ON_WM_RBUTTONDOWN()
    ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDblclk)
END_MESSAGE_MAP()


// CFileTreeView diagnostics

#ifdef _DEBUG
void CFileTreeView::AssertValid() const
{
	CMyTreeView::AssertValid();
}

void CFileTreeView::Dump(CDumpContext& dc) const
{
	CMyTreeView::Dump(dc);
}
#endif //_DEBUG


////////////////////////////////////////////////////////////////////////////////////////////////////
// CFileTreeView message handlers

void CFileTreeView::OnInitialUpdate()
{
	CMyTreeView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
    m_ContextMenu.CreatePopupMenu();
    m_ContextMenu.AppendMenu(MF_STRING, IDM_POPEXPLORER, "파일 위치 열기");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// CFileTreeView message handlers

void CFileTreeView::OnRButtonDown(UINT nFlags, CPoint point)
{
    DEBUG_LOG(">> CFileTreeView::OnRButtonDown()");
    CPoint ptRClick = point;

    if (m_bClickIcon == TRUE) {
        return;
    }

    HTREEITEM	hItem;
    HTREEITEM	hParent;
    CString     strParent;
    CString     strItem;
    CString     strItemPath;
    CTreeCtrl& treeCtrl = this->GetTreeCtrl();

    // 1. 해당 위치의 아이템을 구해서, 그 아이템이 선택되도록
    hItem = treeCtrl.HitTest(point);
    treeCtrl.SelectItem(hItem);
    strItem = treeCtrl.GetItemText(hItem);

    ::ClientToScreen(this->m_hWnd, &ptRClick);
    // 2. 바닥을 찍었으면 그냥 보내고
    if (hItem == NULL) {
        return;
    }

    if (strItem == "Mobigo Secure") {
        return;
    }

    LONG lItem = m_ContextMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, ptRClick.x, ptRClick.y, this);
    
    if (lItem) {
        CString strSelect = "";

        // 1. 해당 위치의 아이템을 구해서, 그 아이템이 선택되도록
        hParent = treeCtrl.GetParentItem(hItem);
        strParent = treeCtrl.GetItemText(hParent);
        
        // 2. 바닥을 찍었으면 그냥 보내고
        if (hItem == NULL) {
            return;
        }

        // 3. 보여줄 파일 경로 가져오기 
        DEBUG_LOG(strItem);
        
        if      ( strParent == "Mobigo Secure" && strItem.Find("Logs"   ) > -1 ) strItem = CString(DEBUG_GET_LOG_PATH())     + "\\";
        else if ( strParent == "Logs"          && strItem.Find("debug"  ) > -1 ) strItem = CString(DEBUG_GET_DEBUG_PATH())   + "\\";
        else if ( strParent == "Logs"          && strItem.Find("error"  ) > -1 ) strItem = CString(DEBUG_GET_ERROR_PATH())   + "\\";
        else if ( strParent == "Logs"          && strItem.Find("light"  ) > -1 ) strItem = CString(DEBUG_GET_LIGHT_PATH())   + "\\";
        else if ( strParent == "Logs"          && strItem.Find("warning") > -1 ) strItem = CString(DEBUG_GET_WARNING_PATH()) + "\\";
        else {
            strSelect = "/select,";
            if      ( strParent.Find("debug")   > -1 )  strItem = CString(DEBUG_GET_DEBUG_PATH())   + "\\" + strItem;
            else if ( strParent.Find("error")   > -1 )  strItem = CString(DEBUG_GET_ERROR_PATH())   + "\\" + strItem;
            else if ( strParent.Find("warning") > -1 )  strItem = CString(DEBUG_GET_WARNING_PATH()) + "\\" + strItem;
            else if ( strParent.Find("light")   > -1 )  strItem = CString(DEBUG_GET_LIGHT_PATH())   + "\\" + strItem;
        }

        strItemPath = strSelect + strItem;
        DEBUG_LOG(strItemPath);
        ::ShellExecute(NULL, "open", "explorer", strItemPath, NULL, SW_SHOW);
    }
}

void CFileTreeView::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    DEBUG_LOG(">> CFileTreeView::OnLButtonDown()");
    // 1. Double Click을 위한 준비
    m_ptLButton = point;

    if (m_bClickIcon == TRUE) {
        return;
    }

    HTREEITEM	hItem;
    CTreeCtrl& treeCtrl = this->GetTreeCtrl();

    // 1. 해당 위치의 아이템을 구해서, 그 아이템이 선택되도록
    hItem = treeCtrl.HitTest(m_ptLButton);
    treeCtrl.SelectItem(hItem);

    // 2. 바닥을 찍었으면 그냥 보내고
    if (hItem == NULL)
        return;

    CXMLTreeView::OnLButtonDown(nFlags, point);
}

void CFileTreeView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
    DEBUG_LOG(">> CFileTreeView::OnNMDblclk()");

    if (m_bClickIcon == TRUE) {
        *pResult = TRUE;
        return;
    }

    HTREEITEM	hItem;
    HTREEITEM	hParent;
    CTreeCtrl&  treeCtrl = this->GetTreeCtrl();
    CString     strParent;
    CString     strItem;
    CString     strItemPath;

    // 1. 해당 위치의 아이템을 구해서, 그 아이템이 선택되도록
    hItem = treeCtrl.HitTest(m_ptLButton);
    treeCtrl.SelectItem(hItem);

    hParent     = treeCtrl.GetParentItem(hItem);
    strParent   = treeCtrl.GetItemText(hParent);
    strItem     = treeCtrl.GetItemText(hItem);

    // 2. 바닥을 찍었으면 그냥 보내고
    DEBUG_LOG( "++   strItem=" + strItem);
    
    if (hItem == NULL) {
        return;
    }
    // 3. 보여줄 파일 경로 가져오기 
    if (     strParent.Find("debug")   > -1)  strItemPath = CString(DEBUG_GET_DEBUG_PATH())   + "\\" + strItem;
    else if (strParent.Find("error")   > -1)  strItemPath = CString(DEBUG_GET_ERROR_PATH())   + "\\" + strItem;
    else if (strParent.Find("light")   > -1)  strItemPath = CString(DEBUG_GET_LIGHT_PATH())   + "\\" + strItem;
    else if (strParent.Find("warning") > -1)  strItemPath = CString(DEBUG_GET_WARNING_PATH()) + "\\" + strItem;
    else { 
        ERROR_LOG( "++   [ERROR] strParent=" + strParent );
        return;
    }

    CUtilSys::OpenFileByEditor( strItemPath );
    *pResult = 0;
}

