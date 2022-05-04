#pragma once


// CMyEditView view

class CMyEditView : public CEditView
{
public:
	DECLARE_DYNCREATE(CMyEditView)
	CFont	m_fontDefault;
	void AddString( LPARAM lParamString );
	CMyEditView();           // protected constructor used by dynamic creation
	virtual ~CMyEditView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	DECLARE_MESSAGE_MAP()

	//{{AFX_MSG(CTesteditView)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnEditChange();
	//}}AFX_MSG

	afx_msg LRESULT OnMsgDebugHandler(	 WPARAM wParam, LPARAM lParam ); 
	afx_msg LRESULT OnMsgWarningHandler( WPARAM wParam, LPARAM lParam ); 
	afx_msg LRESULT OnMsgErrorHandler(	 WPARAM wParam, LPARAM lParam ); 
	afx_msg LRESULT OnMsgWatchHandler(	 WPARAM wParam, LPARAM lParam ); 

public:
	virtual void OnInitialUpdate();
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};


