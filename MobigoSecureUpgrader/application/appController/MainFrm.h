#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "DebugControlBar.h"
#include "TreeControlBar.h"
#include "ProgressStatusBar.h"


////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void		RecalcLayout(BOOL bNotify = TRUE);
	virtual BOOL		PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// member data 
////////////////////////////////////////////////////////////////////////////////

protected:  
	CToolBar			m_wndToolBar;
	CProgressStatusBar	m_wndStatusBar;
public:
	CDebugControlBar	m_wndDebugBar;
	CTreeControlBar		m_wndTreeBar;

////////////////////////////////////////////////////////////////////////////////
// member function 
////////////////////////////////////////////////////////////////////////////////

// get/set
public:
	CDebugControlBar*		GetDebugBar() { return &m_wndDebugBar; }
	CTreeControlBar*		GetTreeBar()  { return &m_wndTreeBar;  }

// init
public:
	void				    InitDebugBar();
	void				    InitTreeBar();
    void                    LoadTreeXML();
    BOOL                    IsOnConfig( CString strConfigID );
    void                    SaveConfig();
    CString                 GetStringOnConfig( CString strConfigID );
};


