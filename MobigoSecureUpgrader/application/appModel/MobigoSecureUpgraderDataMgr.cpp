
////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MobigoSecureUpgraderDataMgr.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CMobigoSecureUpgraderDataMgr::CMobigoSecureUpgraderDataMgr() 
    : CDataMgr()
{
	//-DEBUG_LOG(">> CMobigoSecureUpgraderDataMgr::CMobigoSecureUpgraderDataMgr()");
}

CMobigoSecureUpgraderDataMgr::~CMobigoSecureUpgraderDataMgr() 
{ 
	//-DEBUG_LOG(">> CMobigoSecureUpgraderDataMgr::~CMobigoSecureUpgraderDataMgr()");
}

////////////////////////////////////////////////////////////////////////////////
// life cycle
///////////////////////////////////////////////////////////////////////////////

void CMobigoSecureUpgraderDataMgr::Init()
{
	//-DEBUG_LOG(">> CMobigoSecureUpgraderDataMgr::Init()");
    CDataMgr::Init();

    if( CUtilFile::IsExistPath( MOBIGO_UPGRADE_LIST_PATH ) == TRUE ) {
        CString strUpgradeListJson = CUtilFile::ReadString( MOBIGO_UPGRADE_LIST_PATH );
        m_pUpgradeData = m_pUpgradeDataLoader->Parse( strUpgradeListJson );
    } else {
        m_pUpgradeData = new CUpgradeData();
        m_pUpgradeData->Init();
    }
}

void CMobigoSecureUpgraderDataMgr::Exit()
{
	//-DEBUG_LOG(">> CMobigoSecureUpgraderDataMgr::Exit()");
    CDataMgr::Exit();
}
