#pragma once


#include "SizingTabCtrlBar.h"
// CDebugControlBar
class CDebugControlBar : public CSizingTabCtrlBar
{
	DECLARE_DYNAMIC(CDebugControlBar)

public:
	CDebugControlBar();
	virtual ~CDebugControlBar();
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);


};


