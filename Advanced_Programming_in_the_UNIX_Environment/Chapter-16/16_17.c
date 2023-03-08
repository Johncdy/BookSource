#include "apue.h"
#include <sys/socket.h>
#include <errno.h>
#include <syslog.h>
#include <netdb.h>

#define BUFLEN  128
#define QLEN    10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX   256
#endif

// void daemonize(const char *cmd)
// {
//     int                 i, fd0, fd1, fd2;
//     pid_t               pid;
//     struct rlimit       rl;
//     struct sigaction    sa;

//     umask(0);

//     if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
//         err_quit("%s: can't get file limit", cmd);
    
//     if ((pid = fork()) < 0)
//         err_quit("%s: can't fork", cmd);
//     else if (pid != 0)
//         exit(0);
//     setsid();

//     sa.sa_handler = SIG_IGN;

//     sigemptyset(&sa.sa_mask);
//     sa.sa_flags = 0;
//     if (sigaction(SIGHUP, &sa, NULL) < 0)
//         err_quit("%s: can't ignore SIGHUP", cmd);
//     if ((pid = fork()) < 0)
//         err_quit("%s: can't fork", cmd);
//     else if (pid != 0)
//         exit(0);
    
//     if (chdir("/") < 0)
//         err_quit("%s: can't change directory to /", cmd);
    
//     if (rl.rlim_max == RLIM_INFINITY)
//         rl.rlim_max = 1024;
//     for (i = 0; i < rl.rlim_max; i++)
//         close(i);
    
//     fd0 = open("/dev/null", O_RDWR);
//     fd1 = dup(0);
//     fd2 = dup(0);

//     openlog(cmd, LOG_CONS, LOG_DAEMON);
//     if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
//         syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
//         exit(1);
//     }
// }

int initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen)
{
    int fd;
    int err = 0;

    if ((fd = socket(addr->sa_family, type, 0)) < 0)
        return(-1);
    if (bind(fd, add, alen) < 0)
        goto errout;
    if (type == SOCKET_STREAM || type == SOCKET_SEQPACKET)
        if (listen(fd, qlen) <0)
            goto errout;
    return(fd);
errout:
    err = errno;
    close(fd);
    errno = err;
    return(-1);
}

void serve(int sockfd)
{

    popen("/bin/uptime", "r");
}

int main(int argc, char *argv[])
{
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    int sockfd, err, n;
    char *host;

    if (argc != 1)
        err_quit("usage: ruptimed");
    if ((n = sysconf(SC_HOST_NAME_MAX)) < 0)
        n = HOST_NAME_MAX;
    if ((host = malloc(n)) == NULL)
        err_sys("malloc error");
    if (gethostname(host, n) < 0)
        err_sys("gethostname error");
    daemonize("ruptimed");

    memset(&hint, 0, sizeof(hint));
    hint.ai_sockettype = SOCK_STREAM;
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL:
    hint.ai_next = NULL;
    hint.ai_flags = AI_CANONNAME;
    if ((err = getaddrinfo(argv[1], "ruptime", &hint, &ailist)) != 0) {
        syslog(LOG_ERR, "getaddrinfo error %s", gai_strerror(err));
        exit(1);
    }
    for (aip = ailist; aip != NULL; aip = aip->ai_next) {
        if ((sockfd = initserver(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen, QLEN)) >= 0) {
            serve(sockfd);
            exit(0);
        }
    }

    exit(1);
}