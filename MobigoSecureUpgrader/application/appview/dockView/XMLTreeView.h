#pragma once

#include "msxml.h"
#include "MyTreeView.h"

// CXMLTreeView view

class CXMLTreeView : public CMyTreeView
{
	DECLARE_DYNCREATE(CXMLTreeView)

protected:
	CXMLTreeView();           // protected constructor used by dynamic creation
	virtual ~CXMLTreeView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:	
	BOOL		Load(		CString			strXMLFilePath );

protected:
	DECLARE_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////
// member data
////////////////////////////////////////////////////////////////////////////////
private :
	int			    m_nIndex;
	HTREEITEM		m_hSettingsItem;

////////////////////////////////////////////////////////////////////////////////
// member function
////////////////////////////////////////////////////////////////////////////////

// get/set
private:
	HTREEITEM		GetSettingsItem() { return m_hSettingsItem; };

// XML -> Tree
public :
	BOOL		    XML2Tree(	CString			strXML         );
	void		    TravelDOM(  IXMLDOMElement* pEle, const HTREEITEM &hParent );
	HTREEITEM	    Setting(	IXMLDOMElement* pEle, const HTREEITEM &hParent );
	HTREEITEM       InsertItem( const HTREEITEM		&hParent, 
					    	    IXMLDOMElement		*pEle,
						    	int					nTreeIcon );
	int             IconString2IconIndex(CString strIcon);
	int             Value2Icon( CString strValue );
// Tree -> XML 
public:
	CString			Tree2XML();
	CString			TravelTree( HTREEITEM hParent, int nDepth );

// utiliity
public :
    BOOL            IsOn( CString strID );
    HTREEITEM       FindTreeItem( HTREEITEM hItem, CString strID );
    CString         GetString( CString strID );
};


