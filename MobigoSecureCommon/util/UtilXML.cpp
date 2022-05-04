#include "StdAfx.h"
#include "macroXML.h"
#include "utilxml.h"
#include "util2.h"

CUtilXML::CUtilXML(void)
{
}

CUtilXML::~CUtilXML(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////
// XML Init / Exit

// init
IXMLDOMDocument* CUtilXML::XMLInit( CString strXML )
{
	if( strXML.IsEmpty() ) {
		AfxMessageBox( "Result XML Empty...!!!" );
		return NULL; // error
	}

	// 0. init xml 
	IXMLDOMDocument*	pDOMDoc = NULL;

	HRESULT hr = CoInitialize(NULL);
	
	if (FAILED(hr)) {
		//JOKH// AfxMessageBox( "COM Initiation Failed..." );
		AfxMessageBox( "COM Initiation Failed..." );
		return NULL; 
	}

	hr = CoCreateInstance(	CLSID_DOMDocument, 
							NULL, 
							CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, 
							IID_IXMLDOMDocument, 
							(LPVOID*)&pDOMDoc );

	if( !SUCCEEDED(hr) )
	{
		CString code;
		code.Format( "XML Document Initiation Failed...[%ld]", hr );
		//JOKH// AfxMessageBox( code );
	}

	if (!pDOMDoc) {
		//JOKH// AfxMessageBox( "XML Document Initiation Failed..." );
		AfxMessageBox( "XML Document Initiation Failed..." );
		return NULL;
	}

	pDOMDoc->put_async(VARIANT_FALSE);

	// 1. load xml
	VARIANT_BOOL bLoad;
	BSTR bstrXML = strXML.AllocSysString();

	//JOKH// AfxMessageBox( strXML );

	hr = pDOMDoc->loadXML( bstrXML, &bLoad );
	::SysFreeString( bstrXML );

	if( !SUCCEEDED(hr) )
	{
		CString code;
		code.Format( "load XML Fail...[%ld]", hr );
		AfxMessageBox( code );
	}

	// 2. return 
	if( bLoad ) 
		return pDOMDoc;

	AfxMessageBox( "Fail Load XML Document" );

	return NULL;
}

// exit
void CUtilXML::XMLExit( IXMLDOMDocument* pDOMDoc, IXMLDOMElement* pEle  )
{
	if( pDOMDoc != NULL ) 		pDOMDoc->Release();
	if( pEle    != NULL )		pEle->Release();

	CoUninitialize();
}

// error
void CUtilXML::XMLError( IXMLDOMDocument* pDOMDoc )
{
	if( pDOMDoc != NULL ) {
		long lLine, lLinePos;
		BSTR	bstrReason	= NULL;
		HRESULT hr			= NULL;

		IXMLDOMParseError*	parseError	= NULL;
		pDOMDoc->get_parseError(&parseError);
		parseError->get_errorCode(&hr);
		parseError->get_line( &lLine );
		parseError->get_linepos(&lLinePos);
		parseError->get_reason(&bstrReason);
			
		CString strMsg;
		strMsg.Format(	_T("Error 0x%.8X on line %d, position %d\r\nReason: %s"), 
						hr, 
						lLine, 
						lLinePos, 
						CString(bstrReason));
		AfxMessageBox( strMsg );
		
		parseError->Release();
		SysFreeString(bstrReason);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// Util

IXMLDOMElement* CUtilXML::GetEle( IXMLDOMDocument* pDOMDoc )
{
	IXMLDOMElement	*pEle	= NULL;
	HRESULT			hr		= pDOMDoc->get_documentElement(&pEle);
	
	if (FAILED(hr) || pEle == NULL) {
		AfxMessageBox( "Empty document!" );
		return NULL;
	}

	return pEle;
}

// Ele         <- current Ele
//   - EleTemp <- return  Ele
IXMLDOMElement* CUtilXML::SkipEle( IXMLDOMElement* pEle )
{
	IXMLDOMElement*	pEleTemp = NULL;
	pEleTemp = CUtilXML::GetFirstChild( pEle );

	if( pEle != NULL )
		pEle->Release();
	
	return pEleTemp;
}

// Root         <- current Ele
//   - Ele      <- return  Ele
IXMLDOMElement* CUtilXML::SkipRoot( IXMLDOMElement* pEle )
{
	if( !CUtilXML::GetTagName( pEle ).CollateNoCase( "Root" ) ) 
		return CUtilXML::SkipEle( pEle );
	return pEle;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Release

// Element
void* CUtilXML::Release(    IXMLDOMElement* pEle )
{
	// 이유를 알 수 없지만 몇번의 Drag & Drop한 후, 
	// 다시 Drag & Drop할 경우, 잘못된 pEle로 인해서 CEditTreeCtrl::isMovable()에서 Error발생

//	if( pEle != NULL ) {
//		pEle->Release();
//		pEle = NULL;
//	}

	return NULL;
}

// NodeList
void* CUtilXML::Release(    IXMLDOMNodeList* pNodeList )
{
	if( pNodeList != NULL ) {
		long lChildLength = 0; 
		pNodeList->get_length( &lChildLength );
		for( long i = 0; i < lChildLength; i++ ) {
			IXMLDOMNode *pNode;
			HRESULT	 hr = pNodeList->get_item( i, &pNode );
			if(SUCCEEDED(hr) && pNode != NULL ) {
				pNode = (IXMLDOMNode*)CUtilXML::Release( pNode );
			}
		}
		
		pNodeList = NULL;
	}

	return NULL;
}

// NameNodeMap
void* CUtilXML::Release( IXMLDOMNamedNodeMap *pNamedNodeMap )
{
	long listLength;
	HRESULT hr = pNamedNodeMap->get_length(&listLength);
	for(long i = 0; i < listLength; i++) {
		IXMLDOMNode* pNode = NULL;
		hr = pNamedNodeMap->get_item(i, &pNode);
		if (SUCCEEDED(hr)) {
			pNode = (IXMLDOMNode*)CUtilXML::Release( pNode );
		}
	}

	return NULL;
}

// Node
void* CUtilXML::Release( IXMLDOMNode *pNode )
{
	if( pNode != NULL ) {
		pNode->Release();
		pNode = NULL;
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Attr

// 20031215 : jkim
//	- 이 함수는 제대로 동작하지 않는다.
//	- 이 함수는 Root의 Attribute는 잘 가져 오지만, Child의 Attribute를 전혀 가지 오지 못한다.
//	- 이유는 모르겠다. 물론 나의 지식부족때문이겠지...
//  - 먼 훗날, 혹시 이 글을 읽는 사람중에 그 이유를 알게되면 나에게 mail을 보내주기 바란다.
//  - 나의 Mail주소는 "jkim@korea.com"이다.
//
//  -> 따라서, 제대로 동작하는 함수인 GetAttr2()을 구현하였다.
CString CUtilXML::GetAttr( IXMLDOMElement* pEle, BSTR bstrAttrName )
{
	CString		strResult = "" ;
	try {
		_variant_t	varValue;
		
		HRESULT hr = pEle->getAttribute( bstrAttrName, &varValue );

		if( SUCCEEDED(hr) ) {
			strResult = (LPCTSTR)(_bstr_t)varValue;

			// debug
			BSTR bstrNodeName;
			pEle->get_nodeName( &bstrNodeName );

			CString strMsg;
			strMsg.Format("< %s > : %s = %s", CString(bstrNodeName), CString(bstrAttrName), strResult );
			AfxMessageBox( strMsg );
		}
		else {
			BSTR bstrNodeName;
			pEle->get_nodeName( &bstrNodeName );

			CString strMsg;
			strMsg.Format("< %s > has no attribute name called \"%s\"", CString(bstrNodeName), CString(bstrAttrName) );
			AfxMessageBox( strMsg );
		}
	} catch(...) {
		CString strMsg;
		strMsg.Format("XML Exception" );
		AfxMessageBox( strMsg );
    }

	return strResult;
}

CString CUtilXML::GetAttr2( IXMLDOMElement *pEle, CString AttrName )
{
	// 1. alloc
	BSTR bstrAttrName = AttrName.AllocSysString();
	// 2. get
	CString strName   = GetAttr2( pEle, bstrAttrName );
	// 3. free
	::SysFreeString(bstrAttrName);
	// 4. return 
	return strName;
}

CString CUtilXML::GetAttr2( IXMLDOMElement *pEle, BSTR bstrAttrName )
{
	if ( pEle == NULL )
		return CString("");
	CString		strResult = "" ;
	HRESULT hr = S_OK;
	BSTR		bstrNodeName;
	_variant_t	varValue , value;

	IXMLDOMNamedNodeMap* pNamedNodeMap = NULL;
	hr = pEle->get_attributes(&pNamedNodeMap);
	if (SUCCEEDED(hr) && pNamedNodeMap != NULL) {
		long listLength;
		hr = pNamedNodeMap->get_length(&listLength);
		for(long i = 0; i < listLength; i++) {
			IXMLDOMNode* pItem = NULL;
			hr = pNamedNodeMap->get_item(i, &pItem);
			if (SUCCEEDED(hr)) {
				hr = pItem->get_nodeName(&bstrNodeName);
				if (SUCCEEDED(hr)) {
					CString strNodeName( bstrNodeName );
					CString strAttrName( bstrAttrName );
					if ( !strNodeName.CompareNoCase( strAttrName ) ) {
						hr = pItem->get_nodeValue(&varValue);
						if (SUCCEEDED(hr)) {
							strResult = (LPCTSTR)(_bstr_t)varValue;
						}
					}
				}
				::SysFreeString(bstrNodeName);
			}
			pItem = (IXMLDOMNode*)CUtilXML::Release( pItem );
		}
	}
	pNamedNodeMap = (IXMLDOMNamedNodeMap*)CUtilXML::Release( pNamedNodeMap );
	
	return strResult;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Special

CString CUtilXML::GetName(	IXMLDOMElement* pEle )
{
	// 1. alloc
	BSTR bstrAttrName = CString( "Name" ).AllocSysString();
	// 2. get
	CString strName   = GetAttr2( pEle, bstrAttrName );
	// 3. free
	::SysFreeString(bstrAttrName);
	// 4. return 
	return strName;
}

CString	CUtilXML::GetValue(		IXMLDOMElement* pEle )
{
	// 1. alloc
	BSTR bstrAttrName = CString( "Value" ).AllocSysString();
	// 2. get
	CString strName   = GetAttr2( pEle, bstrAttrName );
	// 3. free
	::SysFreeString(bstrAttrName);
	// 4. return 
	return strName;
}

int CUtilXML::GetStart(	IXMLDOMElement*  pEle )
{
	// 1. alloc
	BSTR bstrAttrName = CString( "Start" ).AllocSysString();
	// 2. get
	CString strValue   = GetAttr2( pEle, bstrAttrName );
	// 3. free
	::SysFreeString(bstrAttrName);
	// 4. String -> Int
	int nValue = CUtil2::Str2Int( strValue );
	// 5. return 
	return nValue;
}

int CUtilXML::GetEnd(	IXMLDOMElement*  pEle )
{
	// 1. alloc
	BSTR bstrAttrName = CString( "End" ).AllocSysString();
	// 2. get
	CString strValue   = GetAttr2( pEle, bstrAttrName );
	// 3. free
	::SysFreeString(bstrAttrName);
	// 4. String -> Int
	int nValue = CUtil2::Str2Int( strValue );
	// 5. return 
	return nValue;
}

CString CUtilXML::GetCondition(	IXMLDOMElement*  pEle )
{
	// 1. alloc
	BSTR bstrAttrName = CString( "Condition" ).AllocSysString();
	// 2. get
	CString strValue   = GetAttr2( pEle, bstrAttrName );
	// 3. free
	::SysFreeString(bstrAttrName);
	// 4. return 
	return strValue;
}

CString CUtilXML::GetFuncName(	IXMLDOMElement*  pEle )
{
	// 1. alloc
	BSTR bstrAttrName = CString( "FuncName" ).AllocSysString();
	// 2. get
	CString strValue   = GetAttr2( pEle, bstrAttrName );
	// 3. free
	::SysFreeString(bstrAttrName);
	// 4. return 
	return strValue;
}

CString CUtilXML::GetArgs(	IXMLDOMElement*  pEle )
{
	// 1. alloc
	BSTR bstrAttrName = CString( "Args" ).AllocSysString();
	// 2. get
	CString strValue   = GetAttr2( pEle, bstrAttrName );
	// 3. free
	::SysFreeString(bstrAttrName);
	// 4. return 
	return strValue;
}

CString CUtilXML::GetReturn(	IXMLDOMElement*  pEle )
{
	// 1. alloc
	BSTR bstrAttrName = CString( "Return" ).AllocSysString();
	// 2. get
	CString strValue   = GetAttr2( pEle, bstrAttrName );
	// 3. free
	::SysFreeString(bstrAttrName);
	// 4. return 
	return strValue;
}

int CUtilXML::GetLine(	IXMLDOMElement*  pEle )
{
	// 1. alloc
	BSTR bstrAttrName = CString( "Line" ).AllocSysString();
	// 2. get
	CString strValue   = GetAttr2( pEle, bstrAttrName );
	// 3. free
	::SysFreeString(bstrAttrName);
	// 4. Str -> Int
	int nLine = CUtil2::Str2Int( strValue );
	// 5. return 
	return nLine;
}

// Icon
CString CUtilXML::GetIcon(IXMLDOMElement* pEle)
{
	// 1. alloc
	BSTR bstrAttrName = CString( "Icon" ).AllocSysString();
	// 2. get
	CString strIcon = GetAttr2(pEle, bstrAttrName);
	// 3. free
	::SysFreeString(bstrAttrName);
	// 5. return 
	return strIcon;
}

// ID
CString CUtilXML::GetID(IXMLDOMElement* pEle)
{
	// 1. alloc
	BSTR bstrAttrName = CString("ID").AllocSysString();
	// 2. get
	CString strIcon = GetAttr2(pEle, bstrAttrName);
	// 3. free
	::SysFreeString(bstrAttrName);
	// 5. return 
	return strIcon;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Tag

// name
CString CUtilXML::GetTagName( IXMLDOMElement* pEle )
{
	BSTR bstrTagName;
	pEle->get_nodeName( &bstrTagName );
	CString strTagName(bstrTagName);
	::SysFreeString(bstrTagName);

	return strTagName;
}

//////////////////////////////////////////////////////////////////////////////////////////
// PCDATA

CString CUtilXML::GetText( IXMLDOMElement* pEle )
{
	BSTR bstrText;
	pEle->get_text( &bstrText );
	CString strText(bstrText);
	::SysFreeString(bstrText);

	return strText;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Extra

// first child
IXMLDOMElement* CUtilXML::GetFirstChild( IXMLDOMElement* pEle )
{
	IXMLDOMNode *pNodeChild;
	HRESULT		hr = pEle->get_firstChild( &pNodeChild );
	if(SUCCEEDED(hr)) {
		IXMLDOMElement*	pEleChild	= (IXMLDOMElement*)pNodeChild;
		return pEleChild;
	}

	return NULL;
}

IXMLDOMElement*	CUtilXML::GetChild( IXMLDOMElement* pEle, CString strTagName, CString strAttrName, CString strAttrValue )
{
	BEGIN_XML_FOR_STATEMENT( pEle, pEleChild ) 
	{
		CString strTagNameTemp     = CUtilXML::GetTagName( pEleChild );
		CString strAttrValueTemp   = CUtilXML::GetAttr2( pEleChild, strAttrName );

		if( strTagName.CompareNoCase( strTagNameTemp )== 0 ){
			if( strAttrValue.CompareNoCase( strAttrValueTemp ) == 0 )
				return pEleChild;
		}
	}
	END_XML_FOR_STATEMENT() 

	return NULL;
}

// is exist child
BOOL CUtilXML::IsExistChild( IXMLDOMElement* pEle )
{
	if( pEle != NULL ) {
		VARIANT_BOOL bHasChild;
		HRESULT		 hr = pEle->hasChildNodes( &bHasChild ); 
		if(SUCCEEDED(hr)) {
			if( bHasChild ) { 
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CUtilXML::DeleteEle( IXMLDOMElement *pEleParent, IXMLDOMElement *pEleChild )
{
	IXMLDOMNode *pOldChild = NULL;

	HRESULT hr = pEleParent->removeChild( pEleChild, &pOldChild );

	if( hr == S_OK )
		return TRUE;

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Etc

CString CUtilXML::GetAttr( CString strResultXML, CString strAttr )
{
	if( strResultXML.IsEmpty() ) {
		AfxMessageBox( "Result XML Empty...!!!" );
		return ""; // error
	}

	// 0. init xml 
	IXMLDOMDocument*	document	= NULL;
	IXMLDOMParseError*	parseError	= NULL;
	IXMLDOMElement*		pEle		= NULL;	

	HRESULT hr = CoInitialize(NULL);
	
	if (FAILED(hr)) {
		//JOKH// AfxMessageBox( "COM Initiation Failed..." );
		AfxMessageBox( "COM Initiation Failed..." );
		return ""; 
	}

	hr = CoCreateInstance(	CLSID_DOMDocument, 
							NULL, 
							CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, 
							IID_IXMLDOMDocument, 
							(LPVOID*)&document );
	if (!document) {
		//JOKH// AfxMessageBox( "XML Document Initiation Failed..." );
		AfxMessageBox( "XML Document Initiation Failed..." );
		return "" ;
	}

	document->put_async(VARIANT_FALSE);

	// 1. load xml
	VARIANT_BOOL bLoad;
	BSTR bstrXML = strResultXML.AllocSysString();
	document->loadXML( bstrXML, &bLoad );

	// 2. load
	if( bLoad ) {
		hr = document->get_documentElement(&pEle);
		if (FAILED(hr) || pEle == NULL) {
			AfxMessageBox( "Empty document!" );
			return "";
		}
		
		// 2.1. root에는 skip하고
		if( !CUtilXML::GetTagName( pEle ).CollateNoCase( "Root" ) )  
			pEle = CUtilXML::GetFirstChild( pEle );

		// 2.2. alloc
		BSTR bstrAttrName = strAttr.AllocSysString();
		// 2.3. get
		CString strName   = GetAttr2( pEle, bstrAttrName );
		// 2.4. free
		::SysFreeString(bstrAttrName);
		// 2.5. return 
		return strName;

	} else { // error
		long lLine, lLinePos;
		BSTR bstrReason = NULL;
		
		document->get_parseError(&parseError);
		parseError->get_errorCode(&hr);
		parseError->get_line( &lLine );
		parseError->get_linepos(&lLinePos);
		parseError->get_reason(&bstrReason);
		
		CString strMsg;
		strMsg.Format(	_T("Error 0x%.8X on line %d, position %d\r\nReason: %s"), 
						hr, 
						lLine, 
						lLinePos, 
						CString(bstrReason));
		AfxMessageBox( strMsg );
		SysFreeString(bstrReason);
		return "" ;
	}
	
	return ""; // error
}

//////////////////////////////////////////////////////////////////////////////////////////
// Child Element -> XML

// bOnlyChild 
//	- nDepth  : recursive를 한 횟수( 첫 Ele를 XML에 포함시키지 않기 위해서 사용된다 )
//				-1 설정시, 첫 Ele도 XML에 포함된다.
CString	CUtilXML::Ele2XML( IXMLDOMElement* pEle, int nDepth )
{
	//-DEBUG_LOG( "CUtilXML::Ele2XML()" );

	CString strXML		= "";

	// 0. check
	if( pEle == NULL )
		return "";

	CString strTagName	= CUtilXML::GetTagName( pEle );

	// 1. start tag
	if( 0 < nDepth ) { 
		strXML += "<" + strTagName;
		strXML +=	CUtilXML::Attr2Str( pEle );
		strXML += ">";
	}

	// 2. child xml
	{
		BEGIN_XML_FOR_STATEMENT( pEle, pEleChild )
			strXML += CUtilXML::Ele2XML( pEleChild, nDepth + 1 );
		END_XML_FOR_STATEMENT()
	}

	// 3. end tag
	if( 0 < nDepth ) { 
		strXML += "</" + strTagName + ">\r\n";
	}

	return strXML;
}

CString CUtilXML::Attr2Str( IXMLDOMElement* pEle )
{
	//-DEBUG_LOG( "CUtilXML::Attr2Str()" );

	CString strXML  = "";

	CString strName			= 	CUtilXML::GetName(			pEle );
	CString	strIcon			=	CUtilXML::GetIcon(			pEle );
	CString	strID			= 	CUtilXML::GetID(			pEle );
	CString	strValue		= 	CUtilXML::GetValue(	    	pEle );

	// 1. name
	if( !strName.IsEmpty() )		strXML += " Name=\""		+ strName		+ "\" ";
	// 2. Icon
	if( !strIcon.IsEmpty() )		strXML += " Icon=\""		+ strIcon		+ "\" ";
	// 3. ID
	if( !strID.IsEmpty() )		    strXML += " ID=\""	        + strID	        + "\" ";
	// 4. Value
	if( !strValue.IsEmpty() )		strXML += " Value=\""	    + strValue	    + "\" ";

	return strXML;
}
