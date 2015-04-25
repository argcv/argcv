#include "argcv/sys/signal.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h> // setsid
#include <string.h>
#include <stdio.h>
#include <stdlib.h> // malloc

#include <syslog.h> // syslog
#include <signal.h> // signal
#include <sys/stat.h> // open O_RDONL
#include <fcntl.h> // open

#include <sys/wait.h> // wait

#include <netdb.h>
#include <arpa/inet.h>


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
#endif  // SA_INTERRUPT
    } else {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;  // SVR4, 4.4BSD
#endif  // SA_RESTART
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
