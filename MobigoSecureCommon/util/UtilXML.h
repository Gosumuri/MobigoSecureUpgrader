#pragma once

// #import "msxml.dll" named_guids
#include <msxml.h>
#include <comdef.h>

class AFX_EXT_CLASS CUtilXML
{
public:
	CUtilXML(void);
	~CUtilXML(void);

// XML Init/Exit
	static IXMLDOMDocument* XMLInit(		CString strXML  );
	static void				XMLExit(		IXMLDOMDocument*, IXMLDOMElement* pEle = NULL );
	static void				XMLError(		IXMLDOMDocument*);
// util
	static IXMLDOMElement*  GetEle(			IXMLDOMDocument* );
	static IXMLDOMElement*  SkipEle(		IXMLDOMElement*  );
	static IXMLDOMElement*  SkipRoot(		IXMLDOMElement*  );
// release
	static void*			Release(		IXMLDOMElement*      );
	static void*			Release(		IXMLDOMNodeList*     );
	static void*			Release(		IXMLDOMNamedNodeMap* );
	static void*			Release(		IXMLDOMNode*         );
// Get Attribute
	static CString			GetAttr(		IXMLDOMElement*,      BSTR bstrAttrName   );
	static CString			GetAttr(		CString strResultXML, CString strAttrName );
	static CString			GetAttr2(		IXMLDOMElement*,      BSTR bstrAttrName   );
	static CString 			GetAttr2(		IXMLDOMElement*,	  CString strAttrName );

// Special
	static CString			GetName(		IXMLDOMElement* );
	static CString			GetValue(		IXMLDOMElement* );
	static int				GetStart(		IXMLDOMElement* );
	static int				GetEnd(			IXMLDOMElement* );
	static CString			GetCondition(	IXMLDOMElement* );
	static CString			GetFuncName(	IXMLDOMElement* );
	static CString			GetArgs(		IXMLDOMElement* );
	static CString			GetReturn(		IXMLDOMElement* );
	static int				GetLine(		IXMLDOMElement* );
	static CString			GetIcon(        IXMLDOMElement* );
	static CString			GetID(          IXMLDOMElement* );

// GetTag
	static CString			GetTagName(		IXMLDOMElement* );
// Get Text
	static CString			GetText(		IXMLDOMElement* );

// Get first Child
	static IXMLDOMElement*	GetFirstChild(	IXMLDOMElement* );
	static BOOL				IsExistChild(	IXMLDOMElement* );
	static BOOL				DeleteEle(		IXMLDOMElement*, IXMLDOMElement* );
	static IXMLDOMElement*	GetChild(		IXMLDOMElement*, CString strTagName, CString strAttrName, CString strAttrValue );
// Ele->XML
	static CString			Ele2XML(		IXMLDOMElement*, int nDepth = -1 );
	static CString			Attr2Str(		IXMLDOMElement* );
};
