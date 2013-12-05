#include <stdio.h>
#include <signal.h>

#include "daemon.h"


int save_pid( const char* path )
{

  pid_t pid = getpid(); // allways succeed

  FILE* f = fopen(path,"w");
  if( f == NULL )
    {
      return -1;
    }
  
  int retval;
  if( fprintf( f, "%d", pid ) < 0 )
    {
      retval = -2;
    }
  else
    {
      retval = 0;
    }
  
  if( fclose( f ) != 0 )
    {
      if( retval == 0 )
	{
	  return -3;
	}
      else
	{
	  return retval; // -2
	}
    }
  else
    {
      return retval;
    }

}


int main ( int argc, char** argv )
{
  int signo;
  sigset_t ss;
  int has_continue = 1;


  //
  // goto back ground
  //
  if( daemon(0, 0) != 0 )
    {
      printf("failed to fork process.\n");
      return -1;
    }


  //
  // save pid file
  //
  if( argc < 2 || save_pid(argv[1]) != 0 )
    {
      printf("failed to save pid.\n");
      return -2;
    }


  //
  // regist signal
  //
  if( sigaddset(&ss, SIGHUP) != 0 )
    {
      printf("failed to sigaddset SIGHUP.\n");
      return -3;
    }

  if( sigaddset(&ss, SIGTERM) != 0 )
    {
      printf("failed to sigaddset SIGTERM.\n");
      return -4;
    }

  if( sigprocmask(SIG_BLOCK, &ss, NULL) != 0 )
    {
      printf("failed to sigprocmask.\n");
      return -5;
    }


  //
  // start 
  //
  if( argc < 3 || dstart(argv[2]) != 0 )
    {
      printf("failed to start.\n");
      return -6;
    }

  do
    {
      if (sigwait(&ss, &signo) == 0)
	{
	  switch(signo)
	    {
	    case SIGTERM: // stop
	      {
		if( dstop() != 0 )
		  {
		    // trap here.
		  }
		has_continue = 0;
		break;
	      }
	    case SIGHUP: // stop and start -> restart
	      {
		if( dstop() !=0 )
		  {
		    printf("failed to stop.\n");
		    return -7;
		  }
		if( dstart(argv[2]) != 0 )
		  {
		    printf("failed to restart.\n");
		    return -8;
		  }
		break;
	      }
	    }
	} 
    } while( has_continue );


}
