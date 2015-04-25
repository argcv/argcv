#include "argcv/sys/daemon.h"

#include <unistd.h>  // setsid
#include <syslog.h>    // syslog
#include <signal.h>    // signal
#include <sys/stat.h>  // open O_RDONL
#include <fcntl.h>     // open

int daemon_proc = 0;  // for err_XXX() functions
char proj_work_dir[1024] = "/";
int max_fd = 64;

int daemon_init(const char* ident) {
    openlog(ident, LOG_PID, LOG_USER);
    // openlog(ident,LOG_CONS|LOG_PID,LOG_USER);
    syslog(LOG_INFO, "daemon starting");
    int i;
    pid_t pid;

    if ((pid = fork()) < 0) {
        return -1;
    } else if (pid) {
        _exit(0);  // parent terminates
    }

    // child 1 continues ...
    if (setsid() < 0)  // become session leader
    {
        return -1;
    }

    signal(SIGHUP, SIG_IGN);

    if ((pid = fork()) < 0) {
        return -1;
    } else if (pid) {
        _exit(0);
    }
    // child 2 continues ...
    daemon_proc = 1;  // for err_XXX() functions

    syslog(LOG_INFO, "chdir: %s\n", proj_work_dir);

    chdir(proj_work_dir);

    // close off file descriptors
    for (i = 0; i < max_fd; i++) {
        close(i);
    }

    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);

    syslog(LOG_INFO, "daemon started");
    return 0;
}

void daemon_destroy() {
    syslog(LOG_INFO, "daemon stopped");
    closelog();
}
