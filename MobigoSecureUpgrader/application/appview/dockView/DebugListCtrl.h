#pragma once

// CDebugListCtrl
typedef enum {
	COLUMN_INDEX_NO		= 0,
	COLUMN_INDEX_LINE,
	COLUMN_INDEX_DESC,
} EnumColumnIndex;

#define COLUMN_WIDTH_NO			50
#define COLUMN_WIDTH_LINE		70
#define COLUMN_WIDTH_DESC		500

#include "MyListCtrl.h"

class CDebugListCtrl : public CMyListCtrl
{
	DECLARE_DYNAMIC(CDebugListCtrl)
public:
	CDebugListCtrl();
	virtual ~CDebugListCtrl();
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void	OnSize(UINT nType, int cx, int cy);
// virtual
public : 
	virtual void	MeasureItem ( LPMEASUREITEMSTRUCT lpMeasureItemStruct );
public:
	void			Init(void);
	int				AddRow( int nLine, CString strDesc );
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};