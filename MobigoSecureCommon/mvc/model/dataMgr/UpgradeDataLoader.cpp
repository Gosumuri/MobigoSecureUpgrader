
////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UpgradeDataLoader.h"

#include "Util2.h"
#include "UtilJson.h"
#include "UtilHttpGet.h"
#include <string>

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CUpgradeDataLoader::CUpgradeDataLoader() 
{
	//-DEBUG_LOG(">> CUpgradeDataLoader::CUpgradeDataLoader()");
}

CUpgradeDataLoader::~CUpgradeDataLoader() 
{ 
	//-DEBUG_LOG(">> CUpgradeDataLoader::~CUpgradeDataLoader()");
}

////////////////////////////////////////////////////////////////////////////////
// life cycle
///////////////////////////////////////////////////////////////////////////////

void CUpgradeDataLoader::Init()
{
	//-DEBUG_LOG(">> CUpgradeDataLoader::Init()");
}

void CUpgradeDataLoader::Exit()
{
	//-DEBUG_LOG(">> CUpgradeDataLoader::Exit()");
}

////////////////////////////////////////////////////////////////////////////////
// loader
///////////////////////////////////////////////////////////////////////////////

CUpgradeData* CUpgradeDataLoader::Load( CString strUrl )
{
	//-DEBUG_LOG(">> CUpgradeDataLoader::Load()");
	//-DEBUG_LOG("++   strUrl=" + strUrl );

    // download 
    CUtilHttpGet httpGet;
	//-DEBUG_LOG("++   CUtilHttpGet::Request( %s )", strUrl.GetString() );
    CString strUpgradeListJson = CUtilHttpGet::Request( strUrl );
	//-DEBUG_LOG("++   strUpgradeListJson=" + strUpgradeListJson );

    // parse
    CUpgradeData* pUpgradeData = Parse( strUpgradeListJson );
    return pUpgradeData;
}

////////////////////////////////////////////////////////////////////////////////
// parse
///////////////////////////////////////////////////////////////////////////////

CUpgradeData* CUpgradeDataLoader::Parse( CString strUpgradeListJson )
{
	//-DEBUG_LOG(">> CUpgradeDataLoader::Parse()");
    //-DEBUG_LOG("++   strUpgradeListJson.GetLength()=%d", strUpgradeListJson.GetLength() );

    CUpgradeData*   pUpgraderData = new CUpgradeData();
    pUpgraderData->Init();

    JsonBox::Value  jsonParser;
    jsonParser.loadFromString(strUpgradeListJson.GetString());
    JsonBox::Object joRoot = jsonParser.getObject();

    CString         strID       = CUtilJson::get_cstring( joRoot, "id"        );
    CString         strMakeTime = CUtilJson::get_cstring( joRoot, "make_time" );
    JsonBox::Array  jaList      = CUtilJson::get_array(   joRoot, "list"      );

    //-DEBUG_LOG("++   strID        = "   + strID         );
    //-DEBUG_LOG("++   strMakeTime  = "   + strMakeTime   );
    //-DEBUG_LOG("++   jaList.size()= %d", jaList.size() );
    
    pUpgraderData->SetID(       strID       );
    pUpgraderData->SetMakeTime( strMakeTime );

    int i = 0;
    for (auto item : jaList ) {
        //-DEBUG_LOG("++   [%d]", i++ );
        JsonBox::Object joItem = item.getObject();
        CUpgradeItem*   pChildItem  = ParseItem( _T("    "), joItem );
        pUpgraderData->AddUpgradeItem( pChildItem );
    }

    return pUpgraderData;
}

CUpgradeItem* CUpgradeDataLoader::ParseItem( CString strTab, JsonBox::Object joItem )
{
    CUpgradeItem*   pUpgradeItem    = new CUpgradeItem();       
    pUpgradeItem->Init();

    
    CString         strID               = CUtilJson::get_cstring( joItem, "id"                  );
    CString         strIcon             = CUtilJson::get_cstring( joItem, "icon"                );
    CString         strDownloadUrl      = CUtilJson::get_cstring( joItem, "download_url"        );
    CString         strSavePath         = CUtilJson::get_cstring( joItem, "save_path"           );
    int             nSize               = CUtilJson::get_int(     joItem, "size"                );
    CString         strCRC              = CUtilJson::get_cstring( joItem, "crc"                 );
    BOOL            bEncryption         = CUtilJson::get_bool(    joItem, "encryption"          );
    CString         strOption           = CUtilJson::get_cstring( joItem, "option"              );
    BOOL            bMandatory          = CUtil2::Option2Bool(    strOption                     );
    CString         strVersion          = CUtilJson::get_cstring( joItem, "version"             );
    CString         strApkVersion       = CUtilJson::get_cstring( joItem, "apk_version"         );
    BOOL            bDownloadComplete   = CUtilJson::get_bool(    joItem, "download_complete"   );
    BOOL            bUnzipComplete      = CUtilJson::get_bool(    joItem, "unzip_complete"      );

    JsonBox::Array  jaList              = CUtilJson::get_array(   joItem, "list"                );

    //-DEBUG_LOG("++   %sstrID              = %s", strTab.GetString(), strID.GetString()            );
    //-DEBUG_LOG("++   %sstrIcon            = %s", strTab.GetString(), strIcon.GetString()          );
    //-DEBUG_LOG("++   %sstrDownloadUrl     = %s", strTab.GetString(), strDownloadUrl.GetString()   );
    //-DEBUG_LOG("++   %sstrSavePath        = %s", strTab.GetString(), strSavePath.GetString()      );
    //-DEBUG_LOG("++   %snSize              = %d", strTab.GetString(), nSize                        );
    //-DEBUG_LOG("++   %sstrCRC             = %s", strTab.GetString(), strCRC.GetString()           );
    //-DEBUG_LOG("++   %sbEncryption        = %s", strTab.GetString(), B2S(bEncryption)             );
    //-DEBUG_LOG("++   %sbMandatory         = %s", strTab.GetString(), B2S(bMandatory)              );
    //-DEBUG_LOG("++   %sstrVersion         = %s", strTab.GetString(), strVersion.GetString()       );
    //-DEBUG_LOG("++   %sstrApkVersion      = %s", strTab.GetString(), strApkVersion.GetString()    );
    //-DEBUG_LOG("++   %sbDownloadComplete  = %s", strTab.GetString(), B2S(bDownloadComplete)       );

    pUpgradeItem->SetID(                strID               );
    pUpgradeItem->SetIcon(              strIcon             );
    pUpgradeItem->SetDownloadUrl(       strDownloadUrl      );
    pUpgradeItem->SetSavePath(          strSavePath         );
    pUpgradeItem->SetSize(              nSize               );
    pUpgradeItem->SetCRC(               strCRC              );
    pUpgradeItem->SetEncryption(        bEncryption         );
    pUpgradeItem->SetMandatory(         bMandatory          );
    pUpgradeItem->SetVersion(           strVersion          );
    pUpgradeItem->SetApkVersion(        strApkVersion       );
    pUpgradeItem->SetDownloadComplete(  bDownloadComplete   );
    pUpgradeItem->SetUnzipComplete(     bUnzipComplete      );

    //-DEBUG_LOG("++   %sjaList.size()  = %d", strTab.GetString(), jaList.size() );
    int i = 0;
    for (auto item : jaList ) {
        //-DEBUG_LOG("++   %s[%d]", strTab.GetString(), i++ );
        JsonBox::Object joItem = item.getObject();
        CUpgradeItem*   pChildItem  = ParseItem( strTab + _T("    "), joItem );
        pUpgradeItem->AddUpgradeItem( pChildItem );
    }

    return pUpgradeItem;
}
