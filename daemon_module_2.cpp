#include <stdio.h>
#include <time.h>
#include "daemon_module.h"

int cpp_on_begin( int a, int b);
int cpp_on_run( int a, int b);
int cpp_on_end();

extern "C" {

int on_begin( int a, int b)
{
  return cpp_on_begin(a,b);
}

int on_run( int a, int b)
{
  return cpp_on_run(a,b);
}

int on_end()
{
  return cpp_on_end();
}

}

int cpp_on_begin( int a, int b)
{
  time_t current_time;
  time(&current_time);
  fprintf(stdout,"%d module_2 stdout.run begin\n", (int)current_time);
  fprintf(stderr,"%d module_2 stderr.run begin\n", (int)current_time);
  return 0;
}

int cpp_on_run( int a, int b)
{
  time_t current_time;
  time(&current_time);
  fprintf(stdout,"%d module_2 stdout.run !!\n", (int)current_time);
  fprintf(stderr,"%d module_2 stderr.run !!\n", (int)current_time);
  return 0;
}

int cpp_on_end()
{
  time_t current_time;
  time(&current_time);
  fprintf(stdout,"%d module_2 stdout.run end\n", (int)current_time);
  fprintf(stderr,"%d module_2 stderr.run end\n", (int)current_time);
  return 0;
}
