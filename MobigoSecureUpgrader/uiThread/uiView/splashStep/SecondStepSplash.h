#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "StepSplash.h"
#include "DownloadDlg.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////

class CSecondStepSplash : public CStepSplash
{
public:
	CSecondStepSplash();
	virtual ~CSecondStepSplash();

////////////////////////////////////////////////////////////////////////////////
// member data
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// member function
////////////////////////////////////////////////////////////////////////////////

public :
    void    Step();

private : 
    BOOL    ShowDownloadDlg();
    void    AddUrlPathIntoDialog( CUpgradeData* pUpgradeData, CDownloadDlg *pDlg );
};