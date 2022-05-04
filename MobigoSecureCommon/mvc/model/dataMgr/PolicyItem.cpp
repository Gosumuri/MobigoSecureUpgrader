
////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PolicyItem.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CPolicyItem::CPolicyItem() 
{
	//-DEBUG_LOG(">> CPolicyItem::CPolicyItem()");

    m_strDelayTime    = "";
    m_strCheckTime    = "";
    m_strCheckVersion = "";
    m_strUpgradeAlert = "";
    m_paUpgradeID     = NULL;
}

CPolicyItem::~CPolicyItem() 
{ 
	//-DEBUG_LOG(">> CPolicyItem::~CPolicyItem()");
}

////////////////////////////////////////////////////////////////////////////////
// life cycle
///////////////////////////////////////////////////////////////////////////////

void CPolicyItem::Init()
{
	//-DEBUG_LOG(">> CPolicyItem::Init()");

    m_strDelayTime    = "";
    m_strCheckTime    = "";
    m_strCheckVersion = "";
    m_strUpgradeAlert = "";

    if( m_paUpgradeID == NULL ) {
        m_paUpgradeID = new CMyStringArray();
    }  
}

void CPolicyItem::Exit()
{
	//-DEBUG_LOG(">> CPolicyItem::Exit()");

    if( m_paUpgradeID != NULL ) {
        m_paUpgradeID->RemoveAll();
        delete m_paUpgradeID;
        m_paUpgradeID = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////
// dump functin
///////////////////////////////////////////////////////////////////////////////

void CPolicyItem::Dump( CString strTab )
{
	//-DEBUG_LOG(">> CPolicyItem::Dump()");
    m_strDelayTime    = "";
    m_strCheckTime    = "";
    m_strCheckVersion = "";
    m_strUpgradeAlert = "";
    m_paUpgradeID     = NULL;

	//-DEBUG_LOG("++   %sm_strDelayTime     = %s", (LPCSTR)strTab, (LPCSTR)m_strDelayTime     );
	//-DEBUG_LOG("++   %sm_strCheckTime     = %s", (LPCSTR)strTab, (LPCSTR)m_strCheckTime     );
	//-DEBUG_LOG("++   %sm_strCheckVersion  = %s", (LPCSTR)strTab, (LPCSTR)m_strCheckVersion  );
	//-DEBUG_LOG("++   %sm_strUpgradeAlert  = %s", (LPCSTR)strTab, (LPCSTR)m_strUpgradeAlert  );

    if( m_paUpgradeID != NULL ) {
	    //-DEBUG_LOG("++   %sm_paUpgradeID->GetCount() = %d", (LPCSTR)strTab, m_paUpgradeID->GetCount() );
        for( int i = 0; i < m_paUpgradeID->GetCount() ; i++ ) {
            CString strUpgradeID = m_paUpgradeID->GetAt( i );
            //-DEBUG_LOG("++   %s[%d] strUpgradeID=%s", (LPCSTR)strTab, i, (LPCSTR)strUpgradeID );
        }
    } else {
	    //-DEBUG_LOG("++   %sm_paPolicyItem  = NULL", (LPCSTR)strTab                     );
    }
}

////////////////////////////////////////////////////////////////////////////////
// toJson functin
///////////////////////////////////////////////////////////////////////////////

CString CPolicyItem::ToJson( CString strTab )
{
	//-DEBUG_LOG(">> CPolicyItem::ToJson()");
    CString strJson = "";

    strJson += strTab + "{"                                                                                       + "\r\n";
    strJson += strTab + "    \"delay_time\"      : " + "\""+  m_strDelayTime     + "\","                          + "\r\n";
    strJson += strTab + "    \"check_time\"      : " + "\""+  m_strCheckTime     + "\","                          + "\r\n";
    strJson += strTab + "    \"check_version\"   : " + "\""+  m_strCheckVersion  + "\","                          + "\r\n";
    strJson += strTab + "    \"upgrade_alert\"   : " + "\""+  m_strUpgradeAlert  + "\","                          + "\r\n";
    if( m_paUpgradeID != NULL ) {    

        strJson += strTab + "    \"upgrade_list\"            : " +                                                + "\r\n";
        strJson += strTab + "    [" +                                                                             + "\r\n";

        for( int i = 0; i < m_paUpgradeID->GetCount() ; i++ ) {
            CString strUpgradeID = m_paUpgradeID->GetAt( i );
            strJson += strTab + "        \""+  strUpgradeID  + "\" ";                                              
            if( i == m_paUpgradeID->GetCount() - 1 ) strJson += CString(" ")                                      + "\r\n";
            else                                     strJson += CString(",")                                      + "\r\n";
        }
        strJson += strTab + "    ]" +                                                                             + "\r\n";
    } 
    strJson += strTab +  "}" +                                                                                    + "\r\n";

    return strJson;
}

