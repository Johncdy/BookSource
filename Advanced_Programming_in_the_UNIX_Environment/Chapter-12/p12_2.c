#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>

#define MAXSTRINGSZ 4096

extern char **environ;

int unsetenv_m(const char *name)
{
    if (name == NULL || *name == '\0' || strchr(name, '=') != NULL)
        return -1;
    
    size_t len;
    char **ep;

    len = strlen(name);
    ep = environ;
    while (*ep != NULL)
    {
        if ((strncmp(name, *ep, len) == 0) && ((*ep)[len] == '=')) {
            char **dp = ep;
            do
            {
                dp[0] = dp[1];
            } while (*dp++);
        } else {
            ep++;
        }
    }
    return 0;
}

static char envbuf[MAXSTRINGSZ]; 

char *getenv_m(const char *name)
{
    size_t len;

    len = strlen(name);
    char **ep = environ;
    while (*ep != NULL)
    {
        if ((strncmp(name, *ep, len) == 0) && ((*ep)[len] == '=')) {
            strncpy(envbuf, &(*ep)[len+1], MAXSTRINGSZ-1);
            return envbuf;
        } else {
            ep++;
        }
    }
    return NULL;
}

static char **last_environ;

pthread_mutex_t env_mutex;

static pthread_once_t init_done = PTHREAD_ONCE_INIT;

static void thread_init(void)
{
    pthread_mutexattr_t attr;

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&env_mutex, &attr);
    pthread_mutex_destroy(&attr);
}

int putenv_m(char *string)
{
    char *a = strchr(string, '=');
    if (a == NULL)
        printf("illegal usage:name=value\n");

    pthread_once(&init_done, thread_init);
    pthread_mutex_lock(&env_mutex);
    
    char **ep = environ;
    size_t namelen = a - string;
    size_t varlen = strlen(string);
    size_t valuelen = varlen - namelen;
    size_t esize = 0;
    if (ep != NULL) {
        for (; *ep != NULL; ep++)
        {
            if (!strncmp(string, *ep, namelen) && (*ep)[namelen] == '=') {
                break;
            } else {
                esize++;
            }
        }
    }

    if (ep == NULL || *ep == NULL) {
        char **new_environ;
        uintptr_t ip_last_environ = (uintptr_t)last_environ;
        new_environ = (char **)realloc(last_environ, (esize + 2) * sizeof(char *));
        if (new_environ == NULL)
        {
            pthread_mutex_unlock(&env_mutex);
            return -1;
        }

        if ((uintptr_t)environ != ip_last_environ)
        {
            memcpy((char *)new_environ, environ, esize * sizeof(char *));
        }
        new_environ[esize] = NULL;
        new_environ[esize+1] = NULL;
        ep = new_environ + esize;

        last_environ = environ = new_environ;
    }
    char *np = malloc(varlen);
    memcpy(np, string, namelen);
    np[namelen] = '=';
    memcpy(&np[namelen+1], &string[namelen+1], valuelen);
    *ep = np;

    pthread_mutex_unlock(&env_mutex);
    return 0;
}

int putenv_r(const char *string, char *buf, int buflen, char **new_environ, int new_env_len)
{
    char *a = strchr(string, '=');
    if (a == NULL)
        printf("illegal usage:name=value\n");

    pthread_once(&init_done, thread_init);
    pthread_mutex_lock(&env_mutex);
    
    char **ep = environ;
    size_t namelen = a - string;
    size_t varlen = strlen(string);
    size_t valuelen = varlen - namelen;
    size_t esize = 0;
    if (ep != NULL) {
        for (; *ep != NULL; ep++)
        {
            if (!strncmp(string, *ep, namelen) && (*ep)[namelen] == '=') {
                break;
            } else {
                esize++;
            }
        }
    }

    if (ep == NULL || *ep == NULL) {
        if ((esize + 2) * sizeof(char *) > new_env_len)
            goto err;
        char **new_environ;
        uintptr_t ip_last_environ = (uintptr_t)last_environ;
        new_environ = (char **)realloc(last_environ, (esize + 2) * sizeof(char *));
        if (new_environ == NULL)
            goto err;

        if ((uintptr_t)environ != ip_last_environ)
        {
            memcpy((char *)new_environ, environ, esize * sizeof(char *));
        }
        new_environ[esize] = NULL;
        new_environ[esize+1] = NULL;
        ep = new_environ + esize;

        last_environ = environ = new_environ;
    }
    if (varlen > buflen)
        goto err;
    memcpy(buf, string, namelen);
    buf[namelen] = '=';
    memcpy(&buf[namelen+1], &string[namelen+1], valuelen);
    *ep = buf;

    pthread_mutex_unlock(&env_mutex);
    return 0;

err:
    pthread_mutex_unlock(&env_mutex);
    return -1;
}

int main(void)
{
    // char **ptr;
    // for (ptr = environ; NULL != *ptr; ptr++) {
    //     printf("%s\n", *ptr);
    // }

    // unsetenv_m("GOROOT");

    // for (ptr = environ; NULL != *ptr; ptr++) {
    //     printf("%s\n", *ptr);
    // }
    // putenv_m("CDY=dddddddd");
    // putenv_m("CDYY=yyyyyyyy");
    // putenv_m("GOROOT=!");

    char **new_env = malloc(4096);
    char *buf = malloc(4096);
    putenv_r("CDY=dddddddd", buf, 4096, new_env, 4096);

    char **ptr;
    for (ptr = environ; NULL != *ptr; ptr++) {
        printf("%s\n", *ptr);
    }

    exit(0);
}