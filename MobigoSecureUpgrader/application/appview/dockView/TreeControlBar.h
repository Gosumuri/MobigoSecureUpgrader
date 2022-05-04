#pragma once

#include "SizingTabCtrlBar.h"

// CTreeControlBar
class CTreeControlBar : public CSizingTabCtrlBar
{
	DECLARE_DYNAMIC(CTreeControlBar)
public:
	CTreeControlBar();
	virtual ~CTreeControlBar();
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void		 MoveView( int cx, int cy); 
};


