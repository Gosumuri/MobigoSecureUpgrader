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
 
    //1. ���񽺸� �����Ѵ�.
    schSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if(schSCManager == NULL)
    {
        DWORD  dwError       = ::GetLastError();
        //-printf(dwError,_TEXT("[CService::ServiceCreate_OpenSCManager] : ")) ;
        return dwError;
    }
    
    //2. ������ ���� �ڵ�� ���񽺸� �����Ѵ�.
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
 
    //3. ������ ���񽺿� ���� ���� ������ �������ش�.
    BOOL bServiceConfig = ::ChangeServiceConfig2(schService, SERVICE_CONFIG_DESCRIPTION, &Desc);
    if(bServiceConfig == FALSE)
    {
        CloseServiceHandle(schSCManager);
        CloseServiceHandle(schService);
 
        DWORD  dwError       = ::GetLastError();
        //ErrorMsg(dwError,_TEXT("[CService::ServiceCreate_OpenSCManager] : ")) ;
 
        return dwError;
    }
 
    //4. ���� �ڵ��� �ݾ��ش�.
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
 
    //1. ���񽺸� �����Ѵ�.
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
 
    //2. ���� �̸����� Ư�� ���񽺸� �����Ѵ�.
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
 
    //3. ���񽺸� �����Ѵ�.
    BOOL bStart = ::StartService(schService, 0, NULL);
    if(bStart == FALSE)
    {
        CloseServiceHandle(schSCManager);
        CloseServiceHandle(schService);
 
        DWORD  dwError       = ::GetLastError();
        //-ErrorMsg(dwError,_TEXT("[CService::ServiceStart_StartService] : ")) ;
 
        return dwError;
    }
 
    //4. ���� �ڵ��� ��� �ݾ��ش�.
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
 
    //1. ���񽺸� �����Ѵ�.
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
 
    //2. ���� �̸��� �̿��ؼ� Ư�� ���񽺸� �����Ѵ�.
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
 
    //3. ������ ���¸� ��û�Ѵ�.
    BOOL bQuery = ::QueryServiceStatus(schService, &ss);
    if(bQuery == FALSE)
    {
        CloseServiceHandle(schSCManager);
        CloseServiceHandle(schService);
 
        DWORD  dwError       = ::GetLastError();
        //-ErrorMsg(dwError,_TEXT("[CService::ServiceStop_QueryServiceStatus] : ")) ;
 
        return dwError;
    }
 
    //4. ���� ���°� STOP ���°� �ƴ϶�� STOP�� ��û�Ѵ�.
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
        ::Sleep(2000); //���񽺰� 2�ʾȿ� ���� �Ǿ�� �Ѵ�.
    }
 
    //5. ���� �ڵ��� ��� �ݾ��ش�.
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
 
    //1. ���񽺸� �����Ѵ�.
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
 
    //2. ���� �̸��� �̿��ؼ� Ư�� ���񽺸� �����Ѵ�.
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
 
    //3. Ư�� ���񽺸� �����Ѵ�.
    BOOL bDelete = ::DeleteService(schService);
    if(bDelete == FALSE)
    {
        CloseServiceHandle(schSCManager);
        CloseServiceHandle(schService);
        DWORD  dwError       = ::GetLastError();
        //-ErrorMsg(dwError,_TEXT("[CService::ServiceDelete_OpenService] : ")) ;
 
        return dwError;
    }
 
    //4. ���� �ڵ��� ��� �ݾ��ش�.
    CloseServiceHandle(schSCManager);
    CloseServiceHandle(schService);
    return ERROR_NONE;
 
}