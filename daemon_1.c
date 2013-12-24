#include <stdio.h>

#ifdef _WIN32
 #include <windows.h>
 #include <process.h>
#else
 #include <pthread.h>
 #include <dlfcn.h>
#endif

#include <time.h>

#include "daemon.h"


#ifdef _WIN32

#define usleep(m) Sleep(m/1000)

typedef HANDLE pthread_t;
typedef void pthread_attr_t;


void* run(void* p);
UINT WINAPI start_routine_win32(void* p){
  run(p); return 0;
}

int pthread_create(pthread_t* thread, pthread_attr_t* attr, void* (*start_routine) ( void* ), void* arg)
{
  return (*thread = (HANDLE)_beginthreadex(NULL, 0, start_routine_win32, arg, 0, NULL)) ? 0 : -1;
}

int pthread_join(pthread_t th, void **thread_return){
  int result =  (WaitForSingleObject(th, INFINITE) == WAIT_OBJECT_0) ? 0 : -1;
  CloseHandle(th);
  return result;
}

#endif



#ifdef _WIN32

typedef HMODULE dll_handle_t;

dll_handle_t dll_load( const char* path )
{
  // returns NULL, if failed.
  return LoadLibrary( path);
}

void* dll_get_proc(dll_handle_t hdll, const char* name)
{
  // returns NULL, if failed.
  return GetProcAddress(hdll, name);
}

void dll_unload(dll_handle_t hdll)
{
  FreeLibrary(hdll);
}

#else

typedef void* dll_handle_t;

dll_handle_t dll_load( const char* path )
{
  // returns NULL, if failed.
  return dlopen(path, RTLD_LAZY);
}

void* dll_get_proc(dll_handle_t hdll, const char* name)
{
  // returns NULL, if failed.
  return dlsym(hdll, name);
}

void dll_unload(dll_handle_t hdll)
{
  dlclose(hdll);
}

#endif





///
/// configuration
///
typedef struct __conf_t
{
  int redirect_stdout;
  int redirect_stderr;
  char stdout_log_path[1024];
  char stderr_log_path[1024];
  char module_path[1024];
} conf_t;
  
int load_conf( const char* conf_path, conf_t* p_conf )
{
  FILE* f = fopen( conf_path, "r");
  if( f == NULL )
    {
      return -1;
    }
  
  p_conf->stdout_log_path[0] = '\0';
  if(fscanf( f, "%s", p_conf->stdout_log_path ) <= 0 )
    {
      fclose( f );
      return -2;
    }
  if( strcmp( p_conf->stdout_log_path, "0" ) == 0 )
    {
      p_conf->redirect_stdout = 0;
    }
  else
    {
      p_conf->redirect_stdout = 1;
    }

  
  p_conf->stderr_log_path[0] = '\0';
  if(fscanf( f, "%s", p_conf->stderr_log_path ) <= 0 )
    {
      fclose( f );
      return -3;
    }
  if( strcmp( p_conf->stderr_log_path, "0" ) == 0 )
    {
      p_conf->redirect_stderr = 0;
    }
  else
    {
      p_conf->redirect_stderr = 1;
    }
  

  p_conf->module_path[0] = '\0';
  if(fscanf( f, "%s", p_conf->module_path ) <= 0 )
    {
      fclose( f );
      return -4;
    }
  

  fclose( f );
  return 0;
}


typedef int (*on_begin_t)( int a, int b );
typedef int (*on_run_t)( int a, int b );
typedef int (*on_end_t)();


typedef struct __module_t
{
  dll_handle_t hdll;
  on_begin_t on_begin;
  on_run_t on_run;
  on_end_t on_end;
} module_t;

module_t g_module;


///
/// main work
///
conf_t g_conf;
int g_running = 0;

void* run( void* p )
{
  time_t current_time;

  time(&current_time);
  fprintf(stdout,"%d daemon_1 stdout.run begin\n", (int)current_time);
  fprintf(stderr,"%d daemon_1 stderr.run begin\n", (int)current_time);

  g_module.on_begin(0,0);

  while( g_running )
    {
      time(&current_time);
      fprintf(stdout,"%d daemon_1 stdout.run !!\n", (int)current_time);
      fprintf(stderr,"%d daemon_1 stderr.run !!\n", (int)current_time);

      g_module.on_run(0,0);

      usleep(1000000);
    }

  time(&current_time);
  fprintf(stdout,"%d daemon_1 stdout.run end\n", (int)current_time);
  fprintf(stderr,"%d daemon_1 stderr.run end\n", (int)current_time);

  g_module.on_end();

  return NULL;

} 


///
/// control
///
pthread_t g_thread;

int dstart( const char* conf_path )
{
  int n;
  time_t current_time;
  
  if( load_conf( conf_path, &g_conf ) != 0 )
    {
      return -1;
    }


  // path != "0"
  if( g_conf.redirect_stdout )
    {
      freopen(g_conf.stdout_log_path, "a", stdout);
      setvbuf(stdout, NULL, _IONBF, 0);
      //  setvbuf(stdout, NULL, _IOLBF, 128);
    }

  // path != "0"
  if( g_conf.redirect_stderr )
    {
      freopen(g_conf.stderr_log_path, "a", stderr);
      setvbuf(stderr, NULL, _IONBF, 0);
      //  setvbuf(stderr, NULL, _IOLBF, 128);
    }


  g_module.hdll = dll_load( g_conf.module_path );
  if( g_module.hdll == NULL )
    {
      return -3;
    }

  g_module.on_begin = dll_get_proc(g_module.hdll, "on_begin");
  if( g_module.on_begin == NULL ) 
    {
      dll_unload( g_module.hdll );
      return -4;
    }

  g_module.on_run = dll_get_proc(g_module.hdll, "on_run");
  if( g_module.on_run == NULL )
    {
      dll_unload( g_module.hdll );
      return -5;
    }

  g_module.on_end = dll_get_proc(g_module.hdll, "on_end");
  if( g_module.on_end == NULL )
    {
      dll_unload( g_module.hdll );
      return -6;
    }


  time(&current_time);
  fprintf(stdout,"%d daemon_1 stdout.dstart\n", (int)current_time);
  fprintf(stderr,"%d daemon_1 stderr.dstart\n", (int)current_time);

  g_running = 1;
  n = pthread_create(&g_thread, NULL, run, NULL);
  if( n == 0 )
    {
      return 0;
    }
  else
    {
      dll_unload( g_module.hdll );
      return -2;
    }

}

int dstop()
{
  int n;
  time_t current_time;

  time(&current_time);
  fprintf(stdout,"%d daemon_1 stdout.dstop\n", (int)current_time);
  fprintf(stderr,"%d daemon_1 stderr.dstop\n", (int)current_time);

  g_running = 0;
  n = pthread_join(g_thread, NULL);

  dll_unload(g_module.hdll);

  if( n == 0 )
    {
      return 0;
    }
  else
    {
      return -1;
    }

}



