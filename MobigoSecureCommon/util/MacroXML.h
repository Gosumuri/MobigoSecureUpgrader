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
		// 2.1. root���� skip�ϰ�
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

	// ���� 1. child�� ���� ������  
	if( SUCCEEDED(hr)) {
		long lChildLength = 0; 
		pNodeList->get_length( &lChildLength );
		for( long i = 0; i < lChildLength; i++ ) {
			IXMLDOMNode *pNodeTemp;
			hr = pNodeList->get_item( i, &pNodeTemp );

			// ���� 2. ����(?)�� �����ϴ� Node�� ��쿡��
			if(SUCCEEDED(hr)) {
				BSTR		bstrNodeType;
				pNodeTemp->get_nodeTypeString(&bstrNodeType);
				
				// ���� 3. element�� ��쿡��( text, comment ������ skip )
				if (!wcscmp(bstrNodeType, L"element")) {
					IXMLDOMElement*	pEleChild	= (IXMLDOMElement*)pNodeTemp;
					
					/////////////////////////
					// --> Your Action     //
					/////////////////////////

					VARIANT_BOOL bHasChild;
					hr = pEleChild->hasChildNodes( &bHasChild );
					if(SUCCEEDED(hr)) {
						// ���� 4. ������ child�� ���� ������( ����1�� �ٽ� �˻��ϴ� ���� -> ���ʿ� )
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
	// ���� 1. child�� ���� ������  
	if( SUCCEEDED(hr)) {
		long lChildLength = 0; 
		pNodeList->get_length( &lChildLength );
		for( long i = 0; i < lChildLength; i++ ) {
			IXMLDOMNode *pNodeTemp;
			hr = pNodeList->get_item( i, &pNodeTemp );
			// ���� 2. ����(?)�� �����ϴ� Node�� ��쿡��
			if(SUCCEEDED(hr)) {
				BSTR		bstrNodeType;
				pNodeTemp->get_nodeTypeString(&bstrNodeType);
				
				// ���� 3. element�� ��쿡��( text, comment ������ skip )
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
