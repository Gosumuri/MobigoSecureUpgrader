#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

#define			FEATURE_MOBIGO_DEBUGGER					    // mobigo debugger 지원 여부 
#define		    FEATURE_DEBUG_DOCKING_BAR   			    // debug용 docking bar 지원 여부
#define	    	FEATURE_LOG_FILE_EXPLORER   			    // log file 를 탐색기 지원 여부 
#define	    	FEATURE_APP_CONFIG_EXPLORER                 // app config 를 탐색기 지원 여부 
#define         FEATURE_REPLACE_SAVE_PATH   			    // "c:\mobigo" -> "c:\mobigo\upgrader"
#define         FEATURE_FILL_SYSTEM_APK_VERSION             // System Apk의 버전 축출 여부
#define         FEATURE_USE_LOCAL_SYSTEM_APK                // 서버로 받은 system apk를 사용하지 않고, local apk사용한다 
#define         FEATURE_RUN_BACKGROUND                      // app은 foreground로 띄우고, upgrader를 background로 띄운 후, 
                                                            // upgrade가 필요할 경우, upgrader를 foreground로 변경하고, app은 종료 시킨다. 
#define         FEATURE_REPLACE_DRIVE_EX_PATH               // C:/mobigo/DriveEx/Common/DriveEx32 -> C:/mobigo/DriveEx
#define         FEATURE_REPLACE_MOBIGO_SECURE_SETUP         // C:/mobigo/upgrader/download/download -> C:/mobigo
#define         FEATURE_PREVENT_DUAL_RUN_DURING_SHORTTIME   // 짧은 시간에 연속된 App의 중복 실행을 방지 한다

#ifndef _DEBUG
    #define         FEATURE_RELEASE_VIEW        		    // relase splash 지원 
#endif

//#define         FEATURE_HTML_MESSAGE_BOX                    // system messag-box대신에 html message-box 사용여부 

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////


