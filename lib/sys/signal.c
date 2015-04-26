#include "argcv/sys/signal.h"

#include <sys/types.h>  // pid_t

#include <stdio.h>  // printf

#include <signal.h>    // signal
#include <sys/wait.h>  // wait

sigfunc* _signal(int signo, sigfunc* func) {
    struct sigaction act;  // bits/sigaction.h
    struct sigaction oact;
    printf("signal called  ...\n");

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);  // signal.h
    act.sa_flags = 0;

    if (signo == SIGALRM)  //
    {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;  // SunOS 4.x
#endif                                 // SA_INTERRUPT
    } else {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;  // SVR4, 4.4BSD
#endif                               // SA_RESTART
    }
    if (sigaction(signo, &act, &oact) < 0) return SIG_ERR;
    return oact.sa_handler;
}

void sig_chld(int signo) {
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
#ifdef __DEBUG__
        if (daemon_proc) {
            syslog(LOG_DEBUG, "child %d terminated \n", pid);
        } else {
            fprintf(stdout, "child %d terminated \n", pid);
        }
#endif  // __DEBUG__
    }
    return;
}
