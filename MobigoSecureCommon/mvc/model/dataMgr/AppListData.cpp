
////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AppItem.h"
#include "AppListData.h"
#include "UtilFile.h"
#include "Util2.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CAppListData::CAppListData() 
{
	//-DEBUG_LOG(">> CAppListData::CAppListData()");

    m_nTotalRecord = 0;
    m_paAppItem    = NULL;
}

CAppListData::~CAppListData() 
{ 
	//-DEBUG_LOG(">> CAppListData::~CAppListData()");
}

////////////////////////////////////////////////////////////////////////////////
// life cycle
///////////////////////////////////////////////////////////////////////////////

void CAppListData::Init()
{
	//-DEBUG_LOG(">> CAppListData::Init()");

    m_nTotalRecord      = 0;

    if( m_paAppItem == NULL ) {
        m_paAppItem = new CMyPtrArray();
    }  
}

void CAppListData::Exit()
{
	//-DEBUG_LOG(">> CAppListData::Exit()");

    m_nTotalRecord      = 0;

    if( m_paAppItem != NULL ) {
        for( int i = m_paAppItem->GetCount() - 1 ; 0 <= i ; i-- ) {
            CAppItem *pItem = (CAppItem*)m_paAppItem->GetAt( i );
            if( pItem != NULL ) {
                pItem->Exit();
                delete pItem;
            }
        }
        m_paAppItem->RemoveAll();
        delete m_paAppItem;
        m_paAppItem = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////
// public funtion
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// dump functin
///////////////////////////////////////////////////////////////////////////////

void CAppListData::Dump( CString strTab )
{
	//-DEBUG_LOG(">> CAppListData::Dump()");
	//-DEBUG_LOG("++   %sm_nTotalRecord          = %d", (LPCSTR)strTab, m_nTotalRecord          );

    if( m_paAppItem != NULL ) {
	    //-DEBUG_LOG("++   %sm_paAppItem->GetCount() = %d", (LPCSTR)strTab, m_paAppItem->GetCount() );
        for( int i = 0; i < m_paAppItem->GetCount() ; i++ ) {
            CAppItem *pItem = (CAppItem*)m_paAppItem->GetAt( i );
            if( pItem != NULL ) {
	            //-DEBUG_LOG("++   %s[%d] CAppItem", (LPCSTR)strTab, i );
                pItem->Dump( strTab + _T("    ") );
            } else {
	            //-DEBUG_LOG("++   %s[%d] CAppItem = NULL", (LPCSTR)strTab, i );
            }
        }
    } else {
	    //-DEBUG_LOG("++   %sm_paAppItem  = NULL", (LPCSTR)strTab                     );
    }
}

////////////////////////////////////////////////////////////////////////////////
// toJson functin
///////////////////////////////////////////////////////////////////////////////

CString CAppListData::ToJson( CString strTab )
{
	//-DEBUG_LOG(">> CAppListData::ToJson()");
    CString strJson = "";

    strJson += strTab + "{"                                                                                                                           + "\r\n";
    strJson += strTab + "    \"totalRecord\"                : " +  CUtil2::Int2Str( m_nTotalRecord ) + ","                                            + "\r\n";

    if( m_paAppItem != NULL && 0 < m_paAppItem->GetCount() ) {    
        strJson += strTab + "    \"list\"                    : "                                                                                       + "\r\n";
        strJson += strTab + "    [" +                                                                                                                  + "\r\n";

        for( int i = 0; i < m_paAppItem->GetCount() ; i++ ) {
            CAppItem *pItem = (CAppItem*)m_paAppItem->GetAt( i );
            if( pItem != NULL ) {
	            CString strItemJson = pItem->ToJson( strTab + _T("        ") );
                strJson += strItemJson;
                if( i == m_paAppItem->GetCount() - 1 ) strJson += strTab + _T("        ")                                                              + "\r\n";
                else                                       strJson += strTab + _T("        ,")                                                         + "\r\n";
            } 
        }
        strJson += strTab + "    ]" +                                                                                                                  + "\r\n";
    } 
    strJson += strTab +  "}" +                                                                                                                         + "\r\n";

    return strJson;
}

