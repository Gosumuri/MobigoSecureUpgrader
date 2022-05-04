////////////////////////////////////////////////////////////////////////////////
// include
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "utilservice.h"


////////////////////////////////////////////////////////////////////////////////
// define
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// macro
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// constructor / destructor
////////////////////////////////////////////////////////////////////////////////

CUtilService::CUtilService(void)
{
}

CUtilService::~CUtilService(void)
{
}

////////////////////////////////////////////////////////////////////////////////
// Install
////////////////////////////////////////////////////////////////////////////////

int CUtilService::Install(   TCHAR * serviceName, 
                             TCHAR * displayServiceName, 
                             TCHAR * exePath, 
                             TCHAR * description)
{
    SC_HANDLE schSCManager = NULL;
    SC_HANDLE schService   = NULL;
 
    //1. 서비스를 오픈한다.
    schSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if(schSCManager == NULL)
    {
        DWORD  dwError       = ::GetLastError();
        //-printf(dwError,_TEXT("[CService::ServiceCreate_OpenSCManager] : ")) ;
        return dwError;
    }
    
    //2. 오픈한 서비스 핸들로 서비스를 생성한다.
    schService = ::CreateService( 
        schSCManager,              // SCManager database 
        serviceName,               // name of service 
        displayServiceName,        // service name to display 
        SERVICE_ALL_ACCESS,        // desired access 
        SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, // service type 
        SERVICE_AUTO_START,      // start type 
        SERVICE_ERROR_NORMAL,      // error control type 
        exePath,                   // service's binary 
        NULL,                      // no load ordering group 
        NULL,                      // no tag identifier 
        NULL,                      // no dependencies 
        NULL,                      // LocalSystem account 
        NULL);                     // no password 
 
    if(schService == NULL)
    {
        CloseServiceHandle(schSCManager);
 
        DWORD  dwError       = ::GetLastError();
        //-ErrorMsg(dwError,_TEXT("[CService::ServiceCreate_CreateService] : ")) ;
        return dwError;
    }
 
    SERVICE_DESCRIPTION Desc;
    Desc.lpDescription = description;
 
    //3. 생성된 서비스에 서비스 설명 내용을 변경해준다.
    BOOL bServiceConfig = ::ChangeServiceConfig2(schService, SERVICE_CONFIG_DESCRIPTION, &Desc);
    if(bServiceConfig == FALSE)
    {
        CloseServiceHandle(schSCManager);
        CloseServiceHandle(schService);
 
        DWORD  dwError       = ::GetLastError();
        //ErrorMsg(dwError,_TEXT("[CService::ServiceCreate_OpenSCManager] : ")) ;
 
        return dwError;
    }
 
    //4. 서비스 핸들모두 닫아준다.
    CloseServiceHandle(schSCManager);
    CloseServiceHandle(schService);
 
    return ERROR_NONE;
}

////////////////////////////////////////////////////////////////////////////////
// Start
////////////////////////////////////////////////////////////////////////////////

int CUtilService::Start(TCHAR * serviceName)
{
    SC_HANDLE schSCManager = NULL;
    SC_HANDLE schService   = NULL;
 
    //1. 서비스를 오픈한다.
    schSCManager = OpenSCManager( 
        NULL,                    // local computer
        NULL,                    // servicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 
 
    if (NULL == schSCManager) 
    {
        DWORD  dwError       = ::GetLastError();
        //-ErrorMsg(dwError,_TEXT("[CService::ServiceStart_OpenSCManager] : ")) ;
 
        return dwError;
    }
 
    //2. 서비스 이름으로 특정 서비스를 오픈한다.
    schService  = OpenService( 
        schSCManager,         // SCM database 
        serviceName,          // name of service 
        SERVICE_ALL_ACCESS);  // full access 
 
    if (schService == NULL)
    { 
        CloseServiceHandle(schSCManager);
 
        DWORD  dwError       = ::GetLastError();
        //-ErrorMsg(dwError,_TEXT("[CService::ServiceStart_OpenService] : ")) ;
 
        return dwError;
    }    
 
    //3. 서비스를 시작한다.
    BOOL bStart = ::StartService(schService, 0, NULL);
    if(bStart == FALSE)
    {
        CloseServiceHandle(schSCManager);
        CloseServiceHandle(schService);
 
        DWORD  dwError       = ::GetLastError();
        //-ErrorMsg(dwError,_TEXT("[CService::ServiceStart_StartService] : ")) ;
 
        return dwError;
    }
 
    //4. 서비스 핸들은 모두 닫아준다.
    CloseServiceHandle(schSCManager);
    CloseServiceHandle(schService);
 
    return ERROR_NONE;
}

////////////////////////////////////////////////////////////////////////////////
// Stop
////////////////////////////////////////////////////////////////////////////////

int CUtilService::Stop(TCHAR * serviceName)
{
    SC_HANDLE schSCManager = NULL;
    SC_HANDLE schService   = NULL;
 
    //1. 서비스를 오픈한다.
    schSCManager = OpenSCManager( 
        NULL,                    // local computer
        NULL,                    // servicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 
 
    if (NULL == schSCManager) 
    {
        DWORD  dwError       = ::GetLastError();
        //-ErrorMsg(dwError,_TEXT("[CService::ServiceStop_OpenSCManager] : ")) ;
 
        return dwError;
    }
 
    //2. 서비스 이름을 이용해서 특정 서비스를 오픈한다.
    schService  = OpenService( 
        schSCManager,         // SCM database 
        serviceName,          // name of service 
        SERVICE_ALL_ACCESS);  // full access 
 
    if (schService == NULL)
    { 
        CloseServiceHandle(schSCManager);
 
        DWORD  dwError       = ::GetLastError();
        //-ErrorMsg(dwError,_TEXT("[CService::ServiceStop_OpenService] : ")) ;
 
        return dwError;
    }    
 
    SERVICE_STATUS ss;
 
    //3. 서비스의 상태를 요청한다.
    BOOL bQuery = ::QueryServiceStatus(schService, &ss);
    if(bQuery == FALSE)
    {
        CloseServiceHandle(schSCManager);
        CloseServiceHandle(schService);
 
        DWORD  dwError       = ::GetLastError();
        //-ErrorMsg(dwError,_TEXT("[CService::ServiceStop_QueryServiceStatus] : ")) ;
 
        return dwError;
    }
 
    //4. 서비스 상태가 STOP 상태가 아니라면 STOP을 요청한다.
    if(ss.dwCurrentState != SERVICE_STOPPED)
    {
        BOOL bControl = ::ControlService(schService, SERVICE_CONTROL_STOP, &ss);
        if(bControl == FALSE)
        {
            CloseServiceHandle(schSCManager);
            CloseServiceHandle(schService);
 
            DWORD  dwError       = ::GetLastError();
            //-ErrorMsg(dwError,_TEXT("[CService::ServiceStop_ControlService] : ")) ;
 
            return dwError;
 
        }
        ::Sleep(2000); //서비스가 2초안에 종료 되어야 한다.
    }
 
    //5. 서비스 핸들은 모두 닫아준다.
    CloseServiceHandle(schSCManager);
    CloseServiceHandle(schService);
 
    return ERROR_NONE;
 
}

////////////////////////////////////////////////////////////////////////////////
// Uninstall
////////////////////////////////////////////////////////////////////////////////

int CUtilService::Uninstall(TCHAR * serviceName)
{
    SC_HANDLE schSCManager = NULL;
    SC_HANDLE schService   = NULL;
 
    //1. 서비스를 오픈한다.
    schSCManager = OpenSCManager( 
        NULL,                    // local computer
        NULL,                    // servicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 
 
    if (NULL == schSCManager) 
    {
        DWORD  dwError       = ::GetLastError();
        //-ErrorMsg(dwError,_TEXT("[CService::ServiceDelete_OpenSCManager] : ")) ;
 
        return dwError;
    }
 
    //2. 서비스 이름을 이용해서 특정 서비스를 오픈한다.
    schService  = OpenService( 
        schSCManager,         // SCM database 
        serviceName,          // name of service 
        SERVICE_ALL_ACCESS);  // full access 
 
    if (schService == NULL)
    { 
        CloseServiceHandle(schSCManager);
 
        DWORD  dwError       = ::GetLastError();
        //-ErrorMsg(dwError,_TEXT("[CService::ServiceDelete_OpenService] : ")) ;
 
        return dwError;
    }    
 
    //3. 특정 서비스를 삭제한다.
    BOOL bDelete = ::DeleteService(schService);
    if(bDelete == FALSE)
    {
        CloseServiceHandle(schSCManager);
        CloseServiceHandle(schService);
        DWORD  dwError       = ::GetLastError();
        //-ErrorMsg(dwError,_TEXT("[CService::ServiceDelete_OpenService] : ")) ;
 
        return dwError;
    }
 
    //4. 서비스 핸들은 모두 닫아준다.
    CloseServiceHandle(schSCManager);
    CloseServiceHandle(schService);
    return ERROR_NONE;
 
}