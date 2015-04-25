#ifndef ARGCV_SYS_SIGNAL_H
#define ARGCV_SYS_SIGNAL_H


#ifdef __cplusplus
extern "C" {
#endif

// ref P. 105 , unix network programming
typedef void sigfunc(int);
sigfunc * _signal(int signo, sigfunc * func);
void sig_chld(int signo);


#ifdef __cplusplus
}
#endif




#endif  //  ARGCV_SYS_SIGNAL_H
