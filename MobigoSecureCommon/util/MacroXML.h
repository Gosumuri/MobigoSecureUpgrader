#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////
// XML Parse

/***********************************************************************************************
{
	// 0. declare
	IXMLDOMDocument* pDOMDoc	= NULL;
	IXMLDOMElement*	 pEle		= NULL;

	// 1. init
	pDOMDoc = CUtilXML::XMLInit( strXML );

	// 2. load
	if( pDOMDoc != NULL ) {
		// 2.1. root에는 skip하고
		pEle = CUtilXML::GetEle( pDOMDoc );	

		////// Your Action ////////////////////////////////////////////////////////
		// pEle = CUtilXML::SkipRoot( pEle );	                                 //                                      // 
		// if( CUtilXML::GetTagName( pEle ).CompareNoCase( "ERROR" ) != 0 ) {    //
		//	                                                                     // 
		//	                                                                     // 
		// }                                                                     // 
		///////////////////////////////////////////////////////////////////////////

	// 3. error
	} else { 
		CUtilXML::XMLError( pDOMDoc );
	}
	// 4. release
	CUtilXML::XMLExit( pDOMDoc, pEle );
}
*************************************************************************************************/

#define BEGIN_XML_PARSER( __strXML ) \
	IXMLDOMDocument* pDOMDoc	= NULL; \
	IXMLDOMElement*	 pEle		= NULL; \
	pDOMDoc = CUtilXML::XMLInit( __strXML ); \
	if( pDOMDoc != NULL ) { \
		pEle = CUtilXML::GetEle( pDOMDoc ); \

#define END_XML_PARSER( __clearDOMDoc ) \
	} else { \
		CUtilXML::XMLError( pDOMDoc ); \
	} \
	if( __clearDOMDoc ) \
		CUtilXML::XMLExit( pDOMDoc, pEle ); \

////////////////////////////////////////////////////////////////////////////////////////////////
// XML Travel

/***********************************************************************************************
{
	IXMLDOMNodeList  *pNodeList;
	HRESULT			 hr			= pEle->get_childNodes( &pNodeList );

	// 조건 1. child가 있을 때에만  
	if( SUCCEEDED(hr)) {
		long lChildLength = 0; 
		pNodeList->get_length( &lChildLength );
		for( long i = 0; i < lChildLength; i++ ) {
			IXMLDOMNode *pNodeTemp;
			hr = pNodeList->get_item( i, &pNodeTemp );

			// 조건 2. 실제(?)로 존재하는 Node일 경우에만
			if(SUCCEEDED(hr)) {
				BSTR		bstrNodeType;
				pNodeTemp->get_nodeTypeString(&bstrNodeType);
				
				// 조건 3. element일 경우에만( text, comment 따위는 skip )
				if (!wcscmp(bstrNodeType, L"element")) {
					IXMLDOMElement*	pEleChild	= (IXMLDOMElement*)pNodeTemp;
					
					/////////////////////////
					// --> Your Action     //
					/////////////////////////

					VARIANT_BOOL bHasChild;
					hr = pEleChild->hasChildNodes( &bHasChild );
					if(SUCCEEDED(hr)) {
						// 조건 4. 하위에 child가 있을 때에만( 조건1을 다시 검사하는 것임 -> 불필요 )
						if( bHasChild ) {
							///////////////////////////////////////////////////////
							// --> Your Action                                   // 
							// example : TravelDOM((IXMLDOMElement*)pEleChild ); //
							///////////////////////////////////////////////////////
						}
					}
				}
			}
			pNodeTemp = (IXMLDOMNode*)CUtilXML::Release( pNodeTemp );
		}
	}
	pNodeList = (IXMLDOMNodeList*)CUtilXML::Release( pNodeList );
}
*************************************************************************************************/

#define BEGIN_XML_TRAVEL( __pEle, __pEleChild ) \
	IXMLDOMNodeList  *pNodeList; \
	HRESULT			 hr			= __pEle->get_childNodes( &pNodeList ); \
	if( SUCCEEDED(hr)) { \
		long lChildLength = 0; \
		pNodeList->get_length( &lChildLength ); \
		for( long nIndex = 0; nIndex < lChildLength; nIndex++ ) { \
			IXMLDOMNode *pNodeTemp; \
			hr = pNodeList->get_item( nIndex, &pNodeTemp ); \
			if(SUCCEEDED(hr)) { \
				BSTR		bstrNodeType; \
				pNodeTemp->get_nodeTypeString(&bstrNodeType); \
				if (!wcscmp(bstrNodeType, L"element")) { \
					IXMLDOMElement*	__pEleChild	= (IXMLDOMElement*)pNodeTemp; \

#define MIDDLE_XML_TRAVEL( __pEleChild ) \
	    			VARIANT_BOOL bHasChild; \
					hr = __pEleChild->hasChildNodes( &bHasChild ); \
					if(SUCCEEDED(hr)) { \
						if( bHasChild ) { \

#define END_XML_TRAVEL( __bRelease ) \
						} \
					} \
				} \
			} \
			if( __bRelease ) \
				pNodeTemp = (IXMLDOMNode*)CUtilXML::Release( pNodeTemp ); \
		} \
	} \
	if( __bRelease ) \
		pNodeList = (IXMLDOMNodeList*)CUtilXML::Release( pNodeList ); \

////////////////////////////////////////////////////////////////////////////////////////////////
// XML For Statement

/***********************************************************************************************
( pEle, pChildEle )
{
	HRESULT			 hr			= pEle->get_childNodes( &pNodeList );
	// 조건 1. child가 있을 때에만  
	if( SUCCEEDED(hr)) {
		long lChildLength = 0; 
		pNodeList->get_length( &lChildLength );
		for( long i = 0; i < lChildLength; i++ ) {
			IXMLDOMNode *pNodeTemp;
			hr = pNodeList->get_item( i, &pNodeTemp );
			// 조건 2. 실제(?)로 존재하는 Node일 경우에만
			if(SUCCEEDED(hr)) {
				BSTR		bstrNodeType;
				pNodeTemp->get_nodeTypeString(&bstrNodeType);
				
				// 조건 3. element일 경우에만( text, comment 따위는 skip )
				if (!wcscmp(bstrNodeType, L"element")) {
					IXMLDOMElement	*pEleChild		= (IXMLDOMElement*)pNodeTemp;

					//////////////////////////////////////////////////////
					// --> Your Action                                  // 
					//////////////////////////////////////////////////////
				}
			}
		}
	}
}
***********************************************************************************************/

#define BEGIN_XML_FOR_STATEMENT( __pEle, __pEleChild ) \
	IXMLDOMNodeList  *pNodeList; \
	HRESULT			 hr			= __pEle->get_childNodes( &pNodeList ); \
	if( SUCCEEDED(hr)) { \
		long lChildLength = 0; \
		pNodeList->get_length( &lChildLength ); \
		for( long nIndex = 0; nIndex < lChildLength; nIndex++ ) { \
			IXMLDOMNode *pNodeTemp; \
			hr = pNodeList->get_item( nIndex, &pNodeTemp ); \
			if(SUCCEEDED(hr)) { \
				BSTR		bstrNodeType; \
				pNodeTemp->get_nodeTypeString(&bstrNodeType); \
				if (!wcscmp(bstrNodeType, L"element")) { \
					IXMLDOMElement	*__pEleChild		= (IXMLDOMElement*)pNodeTemp; \

#define END_XML_FOR_STATEMENT() \
				} \
			} \
		} \
	} \


class AFX_EXT_CLASS CMacroXML
{
public:
	CMacroXML(void);
	~CMacroXML(void);
};
