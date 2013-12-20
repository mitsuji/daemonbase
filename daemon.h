#ifndef __DAEMON_H_
#define __DAEMON_H_

#ifdef __cplusplus
extern "C" {
#endif
  
//
// if succeed return 0
//
int dstart( const char* conf_path );

//
// if succeed return 0
//
int dstop();

#ifdef __cplusplus
}
#endif

#endif
