#pragma once

// CMyTreeView view

class CMyTreeView : public CTreeView
{
	DECLARE_DYNCREATE(CMyTreeView)

protected:
	CMyTreeView();           // protected constructor used by dynamic creation
	virtual ~CMyTreeView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

// Image
public:
	CImageList		m_imgList;
	void			InitIconList();
	void		    AddIcon(int nBitmapID);
// Util
public:
	void			ExpandChildItem( int nDepth, HTREEITEM hItem = NULL );	


	virtual void OnInitialUpdate();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


