#include "apue.h"
#include <termios.h>

#ifdef LINUX
#define OPTSTR "+d:einv"
#else
#define OPTSTR "d:einv"
#endif

static void set_noecho(int);
void        do_driver(char *);
void        loop(int, int);

pid_t pty_fork(int *ptrfdm, char *slave_name, int slave_namesz, const struct termios *slave_termios, const struct winsize *slave_winsize)
{
    int fdm, fds;
    pid_t pid;
    char pts_name[20];

    if ((fdm = ptym_open(pts_name, sizeof(pts_name))) < 0)
        err_sys("can't open master pty: %s, error %d", pts_name, fdm);
    
    if (slave_name != NULL) {
        strncpy(slave_name, pts_name, slave_namesz);
        slave_name[slave_namesz - 1] = '\0';
    }

    if ((pid = fork()) < 0) {
        return(-1);
    } else if (pid == 0) {
        if (setsid() < 0)
            err_sys("setsid error");
        
        if ((fds = ptys_open(pts_name)) < 0)
            err_sys("can't open slave pty");
        close(fdm);

#if defined(BSD)
        if (ioctl(fds, TIOCSCITY, (char *)0) < 0)
            err_sys("TIOCSCITY error");
#endif
        if (slave_termios != NULL) {
            if (tcsetattr(fds, TCSANOW, slave_termios) < 0)
                err_sys("tcsetattr error on slave pty");
        }
        if (slave_winsize != NULL) {
            if (ioctl(fds, TIOCSWINSZ, slave_winsize) < 0)
                err_sys("TIOCSWINSZ error on slave pty");
        }

        if (dup2(fds, STDIN_FILENO) != STDIN_FILENO)
            err_sys("dup2 error to stdin");
        if (dup2(fds, STDOUT_FILENO) != STDOUT_FILENO)
            err_sys("dup2 error to stdout");
        if (dup2(fds, STDERR_FILENO) != STDERR_FILENO)
            err_sys("dup2 error to stderr");
        if (fds != STDIN_FILENO && fds != STDOUT_FILENO && fds != STDERR_FILENO)
            close(fds);
        return(0);
    } else {
        *ptrfdm = fdm;
        return(pid);
    }
}

int main(int argc, char *argv[])
{
    int             fdm, c, ignoreeof, interactive, noecho, verbose;
    pid_t           pid;
    char            *driver;
    char            slave_name[20];
    struct termios  orig_termios;
    struct winsize  size;

    interactive = isatty(STDIN_FILENO);
    ignoreeof = 0;
    noecho = 0;
    verbose = 0;
    dirver = NULL;

    opterr = 0;
    while ((c = getopt(argc, argv, OPTSTR)) != EOF)
    {
        switch (c)
        {
        case 'd':
            driver = optarg;
            break;
        case 'e':
            noecho = 1;
            break;
        case 'i':
            ignoreeof = 1;
            break;
        case 'n':
            interactive = 0;
            break;
        case 'v':
            verbose = 1;
            break;
        case '?':
            err_quit("unrecognized option: -%c", optopt);
        }
    }
    if (optind >= argc)
        err_quit("usage: pty [ -d driver -einv ] program [ arg ... ]");
    
    if (interactive) {
        if (tcgetattr(STDIN_FILENO, &orig_termios) < 0)
            err_sys("tcgetattr error on stdin");
        if (ioctl(STDIN_FILENO, TIOCGWINSZ, (char *) &size) < 0)
            err_sys("TIOCGWINSZ error");
        pid = pty_fork(&fdm, slave_name, sizeof(slave_name), &orig_termios, &size);
    } else {
        pid = pty_fork(&fdm, slave_name, sizeof(slave_name), sizeof(slave_name), NULL, NULL);
    }

    if (pid < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        if (noecho)
            set_noecho(STDIN_FILENO);
        
        if (execvp(argv[optind], &argv[optind]) < 0)
            err_sys("can't execute: %s", argv[optind]);
    }

    if (verbose) {
        fprintf(stderr, "slave name = %s\n", slave_name);
        if (driver != NULL)
            fprintf(stderr, "driver = %s\n", driver);
    }
}