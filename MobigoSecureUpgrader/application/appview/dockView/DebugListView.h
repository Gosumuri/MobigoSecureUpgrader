#pragma once

// CDebugListView form view
#include "resource.h"
#include "DebugListCtrl.h"

class CDebugListView : public CFormView
{
	DECLARE_DYNCREATE(CDebugListView)

protected:
	CDebugListView();           // protected constructor used by dynamic creation
	virtual ~CDebugListView();

public:
	enum { IDD = IDD_DEBUGLISTVIEW };
#ifdef _DEBUG
	virtual void	AssertValid() const;
	virtual void	Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void	OnSize(UINT nType, int cx, int cy);

public:
	CDebugListCtrl	m_lcDebug;
	virtual void OnInitialUpdate();
};


