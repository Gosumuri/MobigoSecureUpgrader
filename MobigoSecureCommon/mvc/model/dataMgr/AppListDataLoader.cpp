
////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AppListDataLoader.h"

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

CAppListDataLoader::CAppListDataLoader() 
{
	//-DEBUG_LOG(">> CAppListDataLoader::CAppListDataLoader()");
}

CAppListDataLoader::~CAppListDataLoader() 
{ 
	//-DEBUG_LOG(">> CAppListDataLoader::~CAppListDataLoader()");
}

////////////////////////////////////////////////////////////////////////////////
// life cycle
///////////////////////////////////////////////////////////////////////////////

void CAppListDataLoader::Init()
{
	//-DEBUG_LOG(">> CAppListDataLoader::Init()");
}

void CAppListDataLoader::Exit()
{
	//-DEBUG_LOG(">> CAppListDataLoader::Exit()");
}

////////////////////////////////////////////////////////////////////////////////
// loader
///////////////////////////////////////////////////////////////////////////////

CAppListData* CAppListDataLoader::Load( CString strUrl )
{
	//-DEBUG_LOG(">> CAppListDataLoader::Load()");
	//-DEBUG_LOG("++   strUrl=" + strUrl );

    //TODO

    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
// parse
///////////////////////////////////////////////////////////////////////////////

CAppListData* CAppListDataLoader::Parse( CString strAppListJson )
{
	//-DEBUG_LOG(">> CAppListDataLoader::Parse()");
    //-DEBUG_LOG("++   strAppListJson.GetLength()=%d", strAppListJson.GetLength() );

    CAppListData*   pAppListData = new CAppListData();
    pAppListData->Init();

    JsonBox::Value  jsonParser;
    jsonParser.loadFromString(strAppListJson.GetString());
    JsonBox::Object joRoot = jsonParser.getObject();

    int nTotalRecord = CUtilJson::get_int( joRoot, "totalRecord" );
    pAppListData->SetTotalRecord( nTotalRecord );

    if( 0 < nTotalRecord ) {
        JsonBox::Array  jaList      = CUtilJson::get_array(   joRoot, "list"      );
    
        int i = 0;
        for (auto item : jaList ) {
            JsonBox::Object joItem = item.getObject();
            CAppItem*   pChildItem  = ParseItem( _T("    "), joItem );
            pAppListData->AddAppItem( pChildItem );

        }
    }

    return pAppListData;
}

CAppItem* CAppListDataLoader::ParseItem( CString strTab, JsonBox::Object joItem )
{
    CAppItem*   pAppItem    = new CAppItem();       
    pAppItem->Init();

    CString strNum        = CUtilJson::get_cstring( joItem, "num"        );
    CString strFileApp    = CUtilJson::get_cstring( joItem, "fileApp"    );
    CString strShortLnk   = CUtilJson::get_cstring( joItem, "shortLnk"   );
    CString strActivation = CUtilJson::get_cstring( joItem, "activation" );
    CString strFrun       = CUtilJson::get_cstring( joItem, "frun"       );

    pAppItem->SetNum(         strNum         );
    pAppItem->SetFileApp(     strFileApp     );
    pAppItem->SetShortLnk(    strShortLnk    );
    pAppItem->SetActivation(  strActivation  );
    pAppItem->SetFrun(        strFrun        );

    return pAppItem;
}
