#include <pwd.h>
#include <stddef.h>
#include <string.h>

struct passwd* getpwnam(const char *name)
{
    struct passwd *ptr;
    setpwent();
    while ((ptr = getpwent()) != NULL)
    {
        if (strcmp(name, ptr->pw_name) == 0)
            break;
    }
    endpwent();
    return ptr;
}

int main(void)
{
    struct passwd *p = getpwnam("root");
    printf("%s-%s-%s-%s-%s-%s-%s\n", p->pw_name, p->pw_passwd, p->pw_uid, p->pw_gid, p->pw_gecos, p->pw_dir, p->pw_shell);
    return 0;
}