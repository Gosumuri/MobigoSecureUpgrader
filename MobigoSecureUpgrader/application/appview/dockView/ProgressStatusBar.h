#if !defined(AFX_PROGRESSSTATUSBAR_H__A6933E35_C47D_11D3_A636_00105A7C2F91__INCLUDED_)
#define AFX_PROGRESSSTATUSBAR_H__A6933E35_C47D_11D3_A636_00105A7C2F91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressStatusBar.h : header file
//

#include "ProgressEx.h"
/////////////////////////////////////////////////////////////////////////////
// CProgressStatusBar window

class CProgressStatusBar : public CStatusBar
{
// Construction
public:
	CProgressStatusBar();
	// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressStatusBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CProgressStatusBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CProgressStatusBar)
	afx_msg void		OnTimer(UINT nIDEvent);
	afx_msg void		OnPaint();
	afx_msg LRESULT		OnProgressStart(	WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT		OnProgressStep(	    WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT		OnProgressEnd(      WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	//}}AFX_MSG
public:
	bool				m_bShow;
	CProgressEx			m_pgrStatus;
	
	void				Start( CString strLabel, int nTotal )	;
	void				Step(int nStep);
	void				End();
// connected
private:
	BOOL				m_bConnected;
public:
	BOOL				GetConnected()					{ return m_bConnected; };
	void				SetConnected( BOOL bConnected )	
	{ 
		m_bConnected = bConnected;
		Invalidate();
	};
// 경과시간
public:
	DWORD				m_dwLapTimeStart;


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSSTATUSBAR_H__A6933E35_C47D_11D3_A636_00105A7C2F91__INCLUDED_)
