#include <stdio.h>

#include "daemon.h"



int main ( int argc, char** argv )
{
  int has_continue = 1;

  if( argc < 2 || dstart( argv[1] ) != 0 )
    {
      printf("failed to start.\n");
      return -1;
    }
  
  do
    {
      switch(getchar())
	{
	case 'q':
	  {
	    if( dstop() != 0 )
	      {
		// trap here.
	      }
	    has_continue = 0;
	    break;
	  }
	case 'r':
	  {
	    if( dstop() != 0 )
	      {
		printf("failed to stop.\n");
		return -2;
	      }
	    if( dstart( argv[1] ) != 0 )
	      {
		printf("failed to restart.\n");
		return -3;
	      }
	    break;
	  }
	} 
    } while( has_continue );

}
