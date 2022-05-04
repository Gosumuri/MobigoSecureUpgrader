#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "afxcmn.h"
#include "afxwin.h"
#include "resource.h"
#include "LogEdit.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

#define	MY_TIMER_DOWNLOAD_START		1
#define	MY_TIMER_DOWNLOAD_END		2

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////


// CDownloadDlg dialog

class CDownloadDlg : public CDialog
{
	DECLARE_DYNAMIC(CDownloadDlg)

public:
	CDownloadDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDownloadDlg();

// Dialog Data
	enum { IDD = IDD_DOWNLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// member data
////////////////////////////////////////////////////////////////////////////////

public :
    CWnd*           m_pParentView;
    static CString  m_strCurFileName;
// control
public:
	// 
	CStatic			m_sttDownloadFileName;
	CProgressCtrl	m_pgDownload;
	CStatic			m_sttThTotal;
	CProgressCtrl	m_pgTotal;
	CStatic			m_sttStatusMessage;
	CLogEdit		m_edDownloadLog;
// property
public:
	CStringArray	m_ayStrSrcPath;
	CStringArray	m_ayStrDescPath;
	CStringArray	m_ayStrSize;
	int				m_nDownloadPathCount;
	int				m_nCurDownloadSize;
	BOOL			m_bDownloadSuccess;

    ULONG		    m_ulPreProgress;

////////////////////////////////////////////////////////////////////////////////
// member function
////////////////////////////////////////////////////////////////////////////////

// Add
public:
	void			AddDownloadPath( CString strSrc, CString strDesc, int nSize );
// Init
public:
	virtual BOOL	OnInitDialog();
	afx_msg void	OnTimer(UINT nIDEvent);
    afx_msg void    OnWindowPosChanging( WINDOWPOS* lpwndpos );
// thread
public:
	static UINT		ThreadDownload( void* pv );
// get / set
public :
    void            SetParentView( CWnd *pParentView )          { m_pParentView = pParentView; };
	void			SetDownloadSuccess( BOOL bDownloadSuccess)	{ m_bDownloadSuccess = bDownloadSuccess; };
	BOOL			IsDownloadSuccess()							{ return m_bDownloadSuccess; };
// etc
public:
	void			ProgressUpdate (	LPCTSTR		szIEMsg, 
										LPCTSTR		szCustomMsg, 
										const int	nPercentDone,
										ULONG		ulProgress );
};
