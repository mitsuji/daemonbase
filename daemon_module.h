#ifndef __DAEMON_MODULE_H__
#define __DAEMON_MODULE_H__

#ifdef __cplusplus
extern "C" {
#endif
  
int on_begin( int a, int b);
int on_run( int a, int b);
int on_end();
  
#ifdef __cplusplus
}
#endif


#endif
