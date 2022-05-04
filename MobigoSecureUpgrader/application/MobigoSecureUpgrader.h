
// MobigoSecureUpgrader.h : MobigoSecureUpgrader ���� ���α׷��� ���� �� ��� ����
//
#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.
#include "MobigoTray.h"
#include "MobigoSecureUpgraderDataMgr.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////

class CMobigoSecureUpgraderApp : public CWinApp
{
public:
	CMobigoSecureUpgraderApp();


// �������Դϴ�.
public:
	virtual BOOL		InitInstance();
	virtual int			ExitInstance();

// �����Դϴ�.
    afx_msg void        OnLogOn();
    afx_msg void        OnLogOff();
    afx_msg void        OnLogClear();
    afx_msg void        OnLogFile();
    afx_msg void        OnSaveConfig();
	afx_msg void		OnAppAbout();
	afx_msg void		OnExit();
    afx_msg void        OnServiceInstall();
    afx_msg void        OnServiceStart();
	afx_msg void		OnServiceStop();
	afx_msg void		OnServiceUninstall();

	DECLARE_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////
// member data
////////////////////////////////////////////////////////////////////////////////

private :
    CMobigoSecureUpgraderDataMgr*    m_pDataMgr;
    CMobigoTray                      m_mobigoTray;

    ULONG_PTR                        m_gdiplusToken;
////////////////////////////////////////////////////////////////////////////////
// member function
////////////////////////////////////////////////////////////////////////////////

public:
// get/set
public:
    CMobigoSecureUpgraderDataMgr*   GetDataMgr()                 { return m_pDataMgr;     }; 
    void                            SetMainWnd( CWnd *pMainWnd ) { m_pMainWnd = pMainWnd; };

	CView*				            GetActiveView();
	CDocument*			            GetActiveDoc();
};

extern CMobigoSecureUpgraderApp g_theApp;
