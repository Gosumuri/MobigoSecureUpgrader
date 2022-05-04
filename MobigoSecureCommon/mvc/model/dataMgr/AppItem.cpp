
////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AppItem.h"
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

CAppItem::CAppItem() 
{
	//-DEBUG_LOG(">> CAppItem::CAppItem()");
}

CAppItem::~CAppItem() 
{ 
	//-DEBUG_LOG(">> CAppItem::~CAppItem()");
}

////////////////////////////////////////////////////////////////////////////////
// life cycle
///////////////////////////////////////////////////////////////////////////////

void CAppItem::Init()
{
	//-DEBUG_LOG(">> CAppItem::Init()");

    m_strNum         = ""; 
    m_strFileApp     = ""; 
    m_strShortLnk    = ""; 
    m_strActivation  = ""; 
    m_strFrun        = ""; 
}

void CAppItem::Exit()
{
	//-DEBUG_LOG(">> CAppItem::Exit()");
}

////////////////////////////////////////////////////////////////////////////////
// public funtion
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// dump functin
///////////////////////////////////////////////////////////////////////////////

void CAppItem::Dump( CString strTab )
{
	//-DEBUG_LOG(">> CAppItem::Dump()");
	//-DEBUG_LOG("++   %sm_strNum        = %s", (LPCSTR)strTab, (LPCSTR)m_strNum        );
	//-DEBUG_LOG("++   %sm_strFileApp    = %s", (LPCSTR)strTab, (LPCSTR)m_strFileApp    );
	//-DEBUG_LOG("++   %sm_strShortLnk   = %s", (LPCSTR)strTab, (LPCSTR)m_strShortLnk   );
	//-DEBUG_LOG("++   %sm_strActivation = %s", (LPCSTR)strTab, (LPCSTR)m_strActivation );
	//-DEBUG_LOG("++   %sm_strFrun       = %s", (LPCSTR)strTab, (LPCSTR)m_strFrun       );
}

////////////////////////////////////////////////////////////////////////////////
// toJson functin
///////////////////////////////////////////////////////////////////////////////

CString CAppItem::ToJson( CString strTab )
{
	//-DEBUG_LOG(">> CAppItem::ToJson()");
    CString strJson = "";

    strJson += strTab + "{"                                                                                                + "\r\n";
    if( m_strNum.IsEmpty()        == FALSE ) strJson += strTab + "    \"num\"        : " + "\""+  m_strNum         + "\"," + "\r\n";
    if( m_strFileApp.IsEmpty()    == FALSE ) strJson += strTab + "    \"fileApp\"    : " + "\""+  m_strFileApp     + "\"," + "\r\n";
    if( m_strShortLnk.IsEmpty()   == FALSE ) strJson += strTab + "    \"shortLnk\"   : " + "\""+  m_strShortLnk    + "\"," + "\r\n";
    if( m_strActivation.IsEmpty() == FALSE ) strJson += strTab + "    \"activation\" : " + "\""+  m_strActivation  + "\"," + "\r\n";
    if( m_strFrun.IsEmpty()       == FALSE ) strJson += strTab + "    \"frun\"       : " + "\""+  m_strFrun        + "\" " + "\r\n";

    strJson += strTab +  "}" +                                                                                             + "\r\n";

    return strJson;
}

