#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "SplashDlg.h"
#include "DownloadDlg.h"
#include "Zip2DirDlg.h"

#include "FirstStepSplash.h"
#include "SecondStepSplash.h"
#include "ThirdStepSplash.h"

////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

#define MY_TIMER_SPLASH_SHOW       1
#define MY_TIMER_SPLASH_CLOSE      2

////////////////////////////////////////////////////////////////////////////////
//
// upgrade�� 5�ܰ�� ����������.
// - �� �ܰ�� ������ �������� ������ �ִ�( ���� ���������� �����ϴ� ) 
// - progressbar�� range�� ���ǻ� 0 ~ 10000���� �����Ѵ�.
//

// |  0 step   |    1 step      |            2 step            |   3 step    |  4 step  | 
// |           |                |                              |             |          |
// |<-- 10% -->|<----- 10% ---->|<-----------  50% ----------->|<--- 30% --->|<-- 0% -->|
// |   (init)  | (upgrade list) |          (download)          |  (unzip)    | (close)  |
/////////////////////////////////////////////////////////////////////////////////

#define SPLASH_PROGRESS_POS_INIT                1000
#define SPLASH_PROGRESS_POS_UPGRADE_LIST        2000
#define SPLASH_PROGRESS_POS_DOWNLOAD            7000
#define SPLASH_PROGRESS_POS_UNZIP              10000

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////

class CUiSplashView
{
protected: // serialization������ ��������ϴ�.
	CUiSplashView();
	virtual ~CUiSplashView();

////////////////////////////////////////////////////////////////////////////////
// member data
////////////////////////////////////////////////////////////////////////////////

protected:
    CWnd                    *m_pWndParent; // �θ� ������ 
    CSplashDlg              *m_pSplashDlg;
    BOOL                    m_bBackground;

    CFirstStepSplash        m_firstStepSplash;    
    CSecondStepSplash       m_secondStepSplash;    
    CThirdStepSplash        m_thirdStepSplash;

////////////////////////////////////////////////////////////////////////////////
// member function
////////////////////////////////////////////////////////////////////////////////

// get/set
public : 
    CSplashDlg*             GetSplash() { return m_pSplashDlg; }

// [0�ܰ�]
public :
    void                    ShowSplash( CWnd* pWndParent, int nLower, int nUpper );

// [1~4�ܰ�]
public : 
    void                    FirstStepSplash();
    void                    SecondStepSplash();
    void                    ThirdStepSplash();

// 4�ܰ�
public :
    void                    FourthStepSplash();
    void                    OnCloseSplash( BOOL bRunApp = TRUE, BOOL BExit = TRUE );

// private : 
private : 
    void                    KillMobigoSecureApp();

};