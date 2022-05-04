
////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PolicyData.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CPolicyData::CPolicyData() 
{
	//-DEBUG_LOG(">> CPolicyData::CPolicyData()");

    m_strID         = "";
    m_strMakeTime   = "";
    m_strVersion    = "";
    m_paPolicyItem  = NULL;
}

CPolicyData::~CPolicyData() 
{ 
	//-DEBUG_LOG(">> CPolicyData::~CPolicyData()");
}

////////////////////////////////////////////////////////////////////////////////
// life cycle
///////////////////////////////////////////////////////////////////////////////

void CPolicyData::Init()
{
	//-DEBUG_LOG(">> CPolicyData::Init()");

    m_strID         = "";
    m_strMakeTime   = "";
    m_strVersion    = "";

    if( m_paPolicyItem == NULL ) {
        m_paPolicyItem = new CMyPtrArray();
    }  
}

void CPolicyData::Exit()
{
	//-DEBUG_LOG(">> CPolicyData::Exit()");

    if( m_paPolicyItem != NULL ) {
        for( int i = m_paPolicyItem->GetCount() - 1 ; 0 <= i ; i-- ) {
            CPolicyItem *pItem = (CPolicyItem*)m_paPolicyItem->GetAt( i );
            if( pItem != NULL ) {
                pItem->Exit();
                delete pItem;
            }
        }
        m_paPolicyItem->RemoveAll();
        delete m_paPolicyItem;
        m_paPolicyItem = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////
// dump functin
///////////////////////////////////////////////////////////////////////////////

void CPolicyData::Dump( CString strTab )
{
	//-DEBUG_LOG(">> CPolicyData::Dump()");
	//-DEBUG_LOG("++   %sm_strMakeTime    = %s", (LPCSTR)strTab, (LPCSTR)m_strMakeTime    );
	//-DEBUG_LOG("++   %sm_strID          = %s", (LPCSTR)strTab, (LPCSTR)m_strID          );
	//-DEBUG_LOG("++   %sm_strVersion     = %s", (LPCSTR)strTab, (LPCSTR)m_strVersion     );

    if( m_paPolicyItem != NULL ) {
        for( int i = m_paPolicyItem->GetCount() - 1 ; 0 <= i ; i-- ) {
            CPolicyItem *pItem = (CPolicyItem*)m_paPolicyItem->GetAt( i );
            pItem->Dump( _T("    ") );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// toJson functin
///////////////////////////////////////////////////////////////////////////////

CString CPolicyData::ToJson( CString strTab )
{
	//-DEBUG_LOG(">> CPolicyData::ToJson()");
    CString strJson = "";

    strJson += strTab + "{"                                                                                     + "\r\n";
    strJson += strTab + "    \"make_time\"               : " + "\""+                   m_strMakeTime    + "\"," + "\r\n";
    strJson += strTab + "    \"id\"                      : " + "\""+                   m_strID          + "\"," + "\r\n";
    strJson += strTab + "    \"version\"                 : " + "\""+                   m_strVersion     + "\"," + "\r\n";
    if( m_paPolicyItem != NULL && 0 < m_paPolicyItem->GetCount() ) {    
        strJson += strTab + "    \"list\"                    : " +                                              + "\r\n";
        strJson += strTab + "    [" +                                                                           + "\r\n";

        for( int i = 0; i < m_paPolicyItem->GetCount() ; i++ ) {
            CPolicyItem *pItem = (CPolicyItem*)m_paPolicyItem->GetAt( i );
            if( pItem != NULL ) {
	            CString strItemJson = pItem->ToJson( strTab + _T("        ") );
                strJson += strItemJson;
                if( i == m_paPolicyItem->GetCount() - 1 ) strJson += strTab + _T("         ")                   + "\r\n";
                else                                      strJson += strTab + _T("        ,")                   + "\r\n";
            } 
        }
        strJson += strTab + "    ]" +                                                                           + "\r\n";
    }
    strJson += strTab + "}" +                                                                                   + "\r\n";

    return strJson;
}


