#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "DataMgr.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////


class CMobigoSecureUpgraderDataMgr : public CDataMgr 
{
public :
	CMobigoSecureUpgraderDataMgr();   // protected constructor used by dynamic creation
	virtual ~CMobigoSecureUpgraderDataMgr();

public:
	void Init();
	void Exit();

////////////////////////////////////////////////////////////////////////////////
// member data
////////////////////////////////////////////////////////////////////////////////

private:

////////////////////////////////////////////////////////////////////////////////
// member function
////////////////////////////////////////////////////////////////////////////////

// get/set
public:

};