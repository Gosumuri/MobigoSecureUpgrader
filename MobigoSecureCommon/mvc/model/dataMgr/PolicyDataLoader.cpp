
////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PolicyDataLoader.h"
#include "UtilHttpGet.h"
#include "UtilJson.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CPolicyDataLoader::CPolicyDataLoader() 
{
	//-DEBUG_LOG(">> CPolicyDataLoader::CPolicyDataLoader()");
}

CPolicyDataLoader::~CPolicyDataLoader() 
{ 
	//-DEBUG_LOG(">> CPolicyDataLoader::~CPolicyDataLoader()");
}

////////////////////////////////////////////////////////////////////////////////
// life cycle
///////////////////////////////////////////////////////////////////////////////

void CPolicyDataLoader::Init()
{
	//-DEBUG_LOG(">> CPolicyDataLoader::Init()");
}

void CPolicyDataLoader::Exit()
{
	//-DEBUG_LOG(">> CPolicyDataLoader::Exit()");
}

////////////////////////////////////////////////////////////////////////////////
// loader
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// loader
///////////////////////////////////////////////////////////////////////////////

CPolicyData* CPolicyDataLoader::Load( CString strUrl )
{
	//-DEBUG_LOG(">> CPolicyDataLoader::Load()");
	//-DEBUG_LOG("++   strUrl=" + strUrl );

    // download 
    CUtilHttpGet httpGet;
	//-DEBUG_LOG("++   CUtilHttpGet::Request( %s )", strUrl.GetString() );
    CString strPolicyListJson = CUtilHttpGet::Request( strUrl );
	//-DEBUG_LOG("++   strPolicyListJson=" + strPolicyListJson );

    // parse
    CPolicyData* pPolicyData = Parse( strPolicyListJson );
    return pPolicyData;
}

////////////////////////////////////////////////////////////////////////////////
// parse
///////////////////////////////////////////////////////////////////////////////

CPolicyData* CPolicyDataLoader::Parse( CString strPolicyListJson )
{
	//-DEBUG_LOG(">> CPolicyDataLoader::Parse()");
    //-DEBUG_LOG("++   strPolicyListJson.GetLength()=%d", strPolicyListJson.GetLength() );

    CPolicyData*   pPolicyData = new CPolicyData();
    pPolicyData->Init();

    JsonBox::Value  jsonParser;
    jsonParser.loadFromString(strPolicyListJson.GetString());
    JsonBox::Object joRoot = jsonParser.getObject();

    CString         strID       = CUtilJson::get_cstring( joRoot, "id"        );
    CString         strMakeTime = CUtilJson::get_cstring( joRoot, "make_time" );
    CString         strVersion  = CUtilJson::get_cstring( joRoot, "version"   );
    JsonBox::Array  jaList      = CUtilJson::get_array(   joRoot, "list"      );

    //-DEBUG_LOG("++   strID        = "   + strID         );
    //-DEBUG_LOG("++   strMakeTime  = "   + strMakeTime   );
    //-DEBUG_LOG("++   strVersion   = "   + strVersion    );
    //-DEBUG_LOG("++   jaList.size()= %d", jaList.size()  );
    
    pPolicyData->SetID(       strID       );
    pPolicyData->SetMakeTime( strMakeTime );
    pPolicyData->SetVersion(  strVersion  );

    int i = 0;
    for (auto item : jaList ) {
        //-DEBUG_LOG("++   [%d]", i++ );
        JsonBox::Object joItem = item.getObject();
        CPolicyItem*   pChildItem  = ParseItem( _T("    "), joItem );
        pPolicyData->AddPolicyItem( pChildItem );
    }

    return pPolicyData;
}

CPolicyItem* CPolicyDataLoader::ParseItem( CString strTab, JsonBox::Object joItem )
{
    CPolicyItem*   pPolicyItem      = new CPolicyItem();       
    pPolicyItem->Init();

    CString         strDelayTime     = CUtilJson::get_cstring( joItem, "delay_time"    );
    CString         strCheckTime     = CUtilJson::get_cstring( joItem, "check_time"    );
    CString         strCheckVersion  = CUtilJson::get_cstring( joItem, "check_version" );
    CString         strUpgradeAlert  = CUtilJson::get_cstring( joItem, "upgrade_alert" );
    JsonBox::Array  jaUpgradeList    = CUtilJson::get_array(   joItem, "upgrade_list"  );

    //-DEBUG_LOG("++   %sstrDelayTime    = %s", strTab.GetString(), strDelayTime.GetString()    );
    //-DEBUG_LOG("++   %sstrCheckTime    = %s", strTab.GetString(), strCheckTime.GetString()    );
    //-DEBUG_LOG("++   %sstrCheckVersion = %s", strTab.GetString(), strCheckVersion.GetString() );
    //-DEBUG_LOG("++   %sstrUpgradeAlert = %s", strTab.GetString(), strUpgradeAlert.GetString() );

    pPolicyItem->SetDelayTime(    strDelayTime    );
    pPolicyItem->SetCheckTime(    strCheckTime    );
    pPolicyItem->SetCheckVersion( strCheckVersion );
    pPolicyItem->SetUpgradeAlert( strUpgradeAlert );

    //-DEBUG_LOG("++   %sjaUpgradeList.size()  = %d", strTab.GetString(), jaUpgradeList.size() );
    int i = 0;
    for (auto item : jaUpgradeList ) {
        CString strUpgradeID = item.getString().c_str() ;
        pPolicyItem->AddUpgradeID( strUpgradeID );
        //-DEBUG_LOG("++   %s[%d] %s", strTab.GetString(), i++, strUpgradeID.GetString() );
    }

    return pPolicyItem;  
}
