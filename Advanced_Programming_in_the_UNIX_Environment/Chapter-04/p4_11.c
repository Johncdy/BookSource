#include "apue.h"
#include <dirent.h>
#include <limits.h>
#include <time.h>

typedef int Myfunc(const char *, const struct stat *, int);

static Myfunc myfunc;

static int myftw(char *, Myfunc *);

static int dopath(Myfunc *);

static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

int main(int argc, char* argv[])
{
    double duration;
    clock_t start, end;
    start = clock();
    int ret;
    if (argc != 2)
    {
        err_quit("usage: ftw <starting-pathname>");
    }

    ret = myftw(argv[1], myfunc);
    ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;
    if (0 == ntot)
        ntot = 1;
    
    printf("regular files  = %71d, %5.2f %%\n", nreg, nreg * 100.0 / ntot);
    printf("directories    = %71d, %5.2f %%\n", ndir, ndir * 100.0 / ntot);
    printf("block special  = %71d, %5.2f %%\n", nblk, nblk * 100.0 / ntot);
    printf("char special   = %71d, %5.2f %%\n", nchr, nchr * 100.0 / ntot);
    printf("FIFOs          = %71d, %5.2f %%\n", nfifo, nfifo * 100.0 / ntot);
    printf("symbolic links = %71d, %5.2f %%\n", nslink, nslink * 100.0 / ntot);
    printf("sockets        = %71d, %5.2f %%\n", nsock, nsock * 100.0 / ntot);
    end = clock();
    duration = (double)(end - start) / CLOCKS_PER_SEC;
    printf("p4_11 costs : %f second\n", duration);
    exit(ret);
}

#define FTW_F 1
#define FTW_D 2
#define FTW_DNR 3
#define FTW_NS 4

static char *filename;
static size_t pathlen;

char *path_alloc(size_t *size)
{
    char *p = NULL;
    if (!size) return NULL;
    p = malloc(256);
    if (p)
        *size = 256;
    else
        *size = 0;
    return p;
}

static int myftw(char *pathname, Myfunc *func)
{
    filename = path_alloc(&pathlen);

    if (pathlen <= strlen(pathname)) {
        pathlen = strlen(pathname) * 2;
        if ((filename = realloc(filename, pathlen)) == NULL) {
            err_sys("realloc error");
        }
    }
    strcpy(filename, pathname);
    dopath(func);
}

static int dopath(Myfunc *func)
{
    struct stat statbuf;
    struct dirent *dirp;
    DIR *dp;
    int ret, n;
    if (lstat(filename, &statbuf) < 0) {
        return (func(filename, &statbuf, FTW_NS));
    }
    if (S_ISDIR(statbuf.st_mode) == 0) {
        return (func(filename, &statbuf, FTW_F));
    }

    if ((ret = func(filename, &statbuf, FTW_D)) != 0) {
        return (ret);
    }
    
    n = strlen(filename);
    if (n + NAME_MAX + 2 > pathlen) {
        pathlen *= 2;
        if ((filename = realloc(filename, pathlen)) == NULL) {
            err_sys("realloc error");
        }
    }
    if ((dp = opendir(filename)) == NULL) {
        return (func(filename, &statbuf, FTW_DNR));
    }
    if (chdir(filename) < 0) {
        err_sys("chdir error");
    }
    while ((dirp = readdir(dp)) != NULL)
    {
        if (strcmp(dirp->d_name, ".") == 0 | strcmp(dirp->d_name, "..") == 0) {
            continue;
        }
        filename = dirp->d_name;
        if ((ret = dopath(func)) != 0)
            break;
    }
    if (chdir("..") < 0) {
        err_sys("chdir .. error");
    }
    if (closedir(dp) < 0) {
        err_ret("can't close directory %s", filename);
    }
    return (ret);
}

static int myfunc(const char *pathname, const struct stat *statptr, int type)
{
    switch (type)
    {
    case FTW_F:
        switch (statptr->st_mode & S_IFMT) {
            case S_IFREG:
                nreg++;
                break;
            case S_IFBLK:
                nblk++;
                break;
            case S_IFCHR:
                nchr++;
                break;
            case S_IFIFO:
                nfifo++;
                break;
            case S_IFLNK:
                nslink++;
                break;
            case S_IFSOCK:
                nsock++;
                break;
            case S_IFDIR:
                err_dump("for S_IFDIR for %s", pathname);
        }
        break;
    case FTW_D:
        ndir++;
        break;
    case FTW_DNR:
        err_ret("can't read directory %s", pathname);
        break;
    case FTW_NS:
        err_ret("stat error for %s", pathname);
        break;
    default:
        err_dump("unknown type %d for pathname %s", type, pathname);
    }
    return 0;
}