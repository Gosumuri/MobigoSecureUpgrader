#pragma once

////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

#define			FEATURE_MOBIGO_DEBUGGER					    // mobigo debugger ���� ���� 
#define		    FEATURE_DEBUG_DOCKING_BAR   			    // debug�� docking bar ���� ����
#define	    	FEATURE_LOG_FILE_EXPLORER   			    // log file �� Ž���� ���� ���� 
#define	    	FEATURE_APP_CONFIG_EXPLORER                 // app config �� Ž���� ���� ���� 
#define         FEATURE_REPLACE_SAVE_PATH   			    // "c:\mobigo" -> "c:\mobigo\upgrader"
#define         FEATURE_FILL_SYSTEM_APK_VERSION             // System Apk�� ���� ���� ����
#define         FEATURE_USE_LOCAL_SYSTEM_APK                // ������ ���� system apk�� ������� �ʰ�, local apk����Ѵ� 
#define         FEATURE_RUN_BACKGROUND                      // app�� foreground�� ����, upgrader�� background�� ��� ��, 
                                                            // upgrade�� �ʿ��� ���, upgrader�� foreground�� �����ϰ�, app�� ���� ��Ų��. 
#define         FEATURE_REPLACE_DRIVE_EX_PATH               // C:/mobigo/DriveEx/Common/DriveEx32 -> C:/mobigo/DriveEx
#define         FEATURE_REPLACE_MOBIGO_SECURE_SETUP         // C:/mobigo/upgrader/download/download -> C:/mobigo
#define         FEATURE_PREVENT_DUAL_RUN_DURING_SHORTTIME   // ª�� �ð��� ���ӵ� App�� �ߺ� ������ ���� �Ѵ�

#ifndef _DEBUG
    #define         FEATURE_RELEASE_VIEW        		    // relase splash ���� 
#endif

//#define         FEATURE_HTML_MESSAGE_BOX                    // system messag-box��ſ� html message-box ��뿩�� 

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// class declare
////////////////////////////////////////////////////////////////////////////////


