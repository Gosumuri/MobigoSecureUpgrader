
// ChildFrm.h : CUiController 클래스의 인터페이스
//

#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "UiReleaseView.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////

//	CMDIChildWnd
//	 + CUiController  <-- 
//	   + CUiTddController 

class CUiController : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CUiController)
public:
	CUiController();

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CUiController();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 생성된 메시지 맵 함수
protected:
    afx_msg LRESULT     OnShowReleaseView(          WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT     OnCompleteFirstStep(        WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT     OnCompleteSecondStep(       WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT     OnCompleteThirdStep(        WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT		OnCompleteDownloadEachItem(	WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT		OnCompleteUnzipEachItem(	WPARAM wParam, LPARAM lParam);

    afx_msg void        OnTimer( UINT_PTR nIDEvent );
	DECLARE_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////
// member data
////////////////////////////////////////////////////////////////////////////////

protected :
    static CUiReleaseView   m_uiReleaseView;
    static BOOL             m_bShownReleaseView;   

////////////////////////////////////////////////////////////////////////////////
// member function
////////////////////////////////////////////////////////////////////////////////

// life cycle 
public:
	virtual	void	Init();
	virtual	void	Exit();

//get/set
public:
    static CUiReleaseView* GetReleaseView() { return &m_uiReleaseView; };
    void                   ShowReleaseView();

// utility
public :
    BOOL                   IsDownloadable( CUpgradeItem* pCurUpgradeItem);
    void                   CheckDownload();
    void                   CheckUnzip();
};
