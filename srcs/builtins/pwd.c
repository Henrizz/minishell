#include "../../includes/minishell.h"

//char *getcwd(char *buffer, size_t size);

void	pwd(void)// TODO: define if is necessary to send success or fail return instead of void
{
    char cwd[PATH_MAX];

    if(getcwd(cwd, PATH_MAX))
        printf("%s\n",cwd);
    else
        printf("pwd: %s", strerror(errno));
}