#pragma once

// CDebugEditView view

#include "MyEditView.h"

class CDebugEditView : public CMyEditView
{
	DECLARE_DYNCREATE(CDebugEditView)

protected:
	CDebugEditView();           // protected constructor used by dynamic creation
	virtual ~CDebugEditView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


