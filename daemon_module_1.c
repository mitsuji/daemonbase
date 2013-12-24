#include <stdio.h>
#include <time.h>
#include "daemon_module.h"


int on_begin( int a, int b)
{
  time_t current_time;
  time(&current_time);
  fprintf(stdout,"%d module_1 stdout.run begin\n", (int)current_time);
  fprintf(stderr,"%d module_1 stderr.run begin\n", (int)current_time);
  return 0;
}

int on_run( int a, int b)
{
  time_t current_time;
  time(&current_time);
  fprintf(stdout,"%d module_1 stdout.run !!\n", (int)current_time);
  fprintf(stderr,"%d module_1 stderr.run !!\n", (int)current_time);
  return 0;
}

int on_end()
{
  time_t current_time;
  time(&current_time);
  fprintf(stdout,"%d module_1 stdout.run end\n", (int)current_time);
  fprintf(stderr,"%d module_1 stderr.run end\n", (int)current_time);
  return 0;
}

