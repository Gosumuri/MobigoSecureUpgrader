#pragma once


// CFileTreeView view

#include "XMLTreeView.h"


class CFileTreeView : public CXMLTreeView
{
	DECLARE_DYNCREATE(CFileTreeView)

protected:
	CFileTreeView();           // protected constructor used by dynamic creation
	virtual ~CFileTreeView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	DECLARE_MESSAGE_MAP()

    CPoint			m_ptLButton;
    BOOL			m_bClickIcon;
    CMenu           m_ContextMenu;
public:
	virtual void OnInitialUpdate();

public :
    afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void	OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void	OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};


