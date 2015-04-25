#ifndef ARGCV_SYS_DAEMON_H
#define ARGCV_SYS_DAEMON_H


#ifdef __cplusplus
extern "C" {
#endif

extern int max_fd;
extern char proj_work_dir[1024];
extern int daemon_proc;  // defined in error.c ?

int daemon_init(const char * ident);
void daemon_destroy();


#ifdef __cplusplus
}
#endif




#endif  //  ARGCV_SYS_DAEMON_H
