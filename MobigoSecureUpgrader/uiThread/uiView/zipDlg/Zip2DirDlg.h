// Zip2DirDlg.h : header file
//

#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "afxcmn.h"
#include "afxwin.h"
#include "LogEdit.h"
#include "msxml.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

#define MY_TIMER_UNZIPPING_START  1
#define MY_TIMER_UNZIPPING_END    2

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////

class CZip2DirDlg : public CDialog
{
// Construction
public:
	CZip2DirDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ZIP2DIR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
    virtual BOOL DestroyWindow();

	afx_msg void	OnTimer(UINT nIDEvent);
    afx_msg void    OnWindowPosChanging( WINDOWPOS* lpwndpos );
	afx_msg LRESULT OnMyMsgProgressZippingHandler( WPARAM wParam, LPARAM lParam ); 
	afx_msg LRESULT OnMyMsgProgressZipInitHandler( WPARAM wParam, LPARAM lParam ); 
	DECLARE_MESSAGE_MAP()
public:
	void			AddZip2Dir( CString strZipPath, CString strDirPath );

////////////////////////////////////////////////////////////////////////////////
// member data
////////////////////////////////////////////////////////////////////////////////

// control
public:
	CStatic			m_sttZipFileName;
	CProgressCtrl	m_pg;				
	CStatic			m_sttFileTh;
	CStatic			m_sttFileTotal;
	
	CProgressCtrl	m_pgTotal;
	CStatic			m_sttZipTotal;
	CStatic			m_sttZipTh;
		
	CLogEdit		m_edLog;

	int				m_nFileTh;			// 몇번째 File인지 나타낸다.
	int				m_nZipTh;			// 몇번째 Zip File인지 나타낸다.

// array path
public:
    CStringArray	m_ayZipPath;
	CStringArray	m_ayDirPath;

    CWnd*           m_pParentView;

////////////////////////////////////////////////////////////////////////////////
// member function
////////////////////////////////////////////////////////////////////////////////
// Init
public:
	void			InitCtrl();
	void			InitEnv();

// get/set
public :
    void            SetParentView( CWnd *pParentView )          { m_pParentView = pParentView; };

// thread
public:
	static UINT		ThreadUnzipping( LPVOID pv );

};
