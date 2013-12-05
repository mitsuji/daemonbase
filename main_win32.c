#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "daemon.h"


char* SERVICE_NAME = "daemonbase";
SERVICE_STATUS_HANDLE serviceStatusHandle;
HANDLE terminateEvent = NULL;
DWORD g_currentState = SERVICE_STOPPED;

char g_conf_path[MAX_PATH];


/**
 *
 */
BOOL InitService()
{
  int n;
  
  n = dstart(g_conf_path);
  if( n == 0 )
    {
      return TRUE;
    }
  else
    {
      return FALSE;
    }
  
}


/**
 *
 */
VOID StopService()
{
  int n;

  n = dstop();
  if( n == 0 )
    {
      // trap here.
    }

  SetEvent(terminateEvent);
}


/**
 *
 */
BOOL SendStatusToSCM (
		      DWORD dwCurrentState,
		      DWORD dwWin32ExitCode,
		      DWORD dwServiceSpecificExitCode,
		      DWORD dwCheckPoint,
		      DWORD dwWaitHint)
{
  BOOL success;
  SERVICE_STATUS serviceStatus;

  serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;

  if( dwCurrentState != 0 )
    {
      serviceStatus.dwCurrentState = dwCurrentState;
      g_currentState = dwCurrentState;
    }  
  else
    {
      serviceStatus.dwCurrentState = g_currentState;
    }

  if (dwCurrentState == SERVICE_START_PENDING)
    serviceStatus.dwControlsAccepted = 0;
  else
    serviceStatus.dwControlsAccepted =
       SERVICE_ACCEPT_STOP
      |SERVICE_ACCEPT_PAUSE_CONTINUE
      |SERVICE_ACCEPT_SHUTDOWN
      ;

  if (dwServiceSpecificExitCode == 0)
    serviceStatus.dwWin32ExitCode = dwWin32ExitCode;
  else
    serviceStatus.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;

  serviceStatus.dwServiceSpecificExitCode = dwServiceSpecificExitCode;
  serviceStatus.dwCheckPoint = dwCheckPoint;
  serviceStatus.dwWaitHint = dwWaitHint;
  
  success = SetServiceStatus (serviceStatusHandle, &serviceStatus);
  if (!success)
    StopService();

  return success;
}



/**
 *
 */
VOID ServiceCtrlHandler (DWORD controlCode)
{
  BOOL success;

  switch(controlCode)
    {
    case SERVICE_CONTROL_STOP:
    case SERVICE_CONTROL_SHUTDOWN:
      success = SendStatusToSCM(SERVICE_STOP_PENDING, NO_ERROR, 0, 1, 5000);
      StopService();
      return;

    case SERVICE_CONTROL_INTERROGATE:
    case SERVICE_CONTROL_PAUSE:
    case SERVICE_CONTROL_CONTINUE:
      break;

    default:
      break;
    }

  SendStatusToSCM(0, NO_ERROR, 0, 0, 0);

}


/**
 *
 */
VOID terminate(DWORD error)
{
  if (terminateEvent)
    CloseHandle(terminateEvent);

  if (serviceStatusHandle)
    SendStatusToSCM(SERVICE_STOPPED, error, 0, 0, 0);

}


/**
 *
 */
VOID ServiceMain(DWORD argc, LPTSTR *argv)
{
  BOOL success;
  
  serviceStatusHandle
    = RegisterServiceCtrlHandler( SERVICE_NAME,
				  (LPHANDLER_FUNCTION)ServiceCtrlHandler );

  if (!serviceStatusHandle)
    {
      terminate(GetLastError());
      return;
    }
  
  success = SendStatusToSCM(SERVICE_START_PENDING, NO_ERROR, 0, 1, 5000);
  if (!success)
    {
      terminate(GetLastError());
      return;
    }

  terminateEvent = CreateEvent (0, TRUE, FALSE, 0);
  if (!terminateEvent)
    {
      terminate(GetLastError());
      return;
    }
  
  success = SendStatusToSCM(SERVICE_START_PENDING, NO_ERROR, 0, 2, 1000);
  if (!success)
    {
      terminate(GetLastError());
      return;
    }
  
  success = InitService();
  if (!success)
    {
      terminate(GetLastError());
      return;
    }
  
  success = SendStatusToSCM(SERVICE_RUNNING, NO_ERROR, 0, 0, 0);
  if (!success)
    {
      terminate(GetLastError());
      return;
    }
  
  
  WaitForSingleObject (terminateEvent, INFINITE);
  terminate(0);
}


/**
 *
 *
 */
VOID main(int argc, char* argv[])
{

  SERVICE_TABLE_ENTRY serviceTable[] =
    {
      { SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION) ServiceMain},
      { NULL, NULL }
    };

  g_conf_path[0] = '\0';
  if ( argc >= 2 )
    {
      strcpy_s(g_conf_path, sizeof(g_conf_path), argv[1] );
    }

  if (!StartServiceCtrlDispatcher(serviceTable))
    {
      printf("Error: StartServiceCtrlDispatcher");
      ExitProcess(GetLastError());
    }

}

