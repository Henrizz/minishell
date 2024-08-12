#include "../../includes/minishell.h"

//char *getcwd(char *buffer, size_t size);

void	pwd(void)// TODO: exit_status??
{
	char cwd[PATH_MAX];

	if(getcwd(cwd, PATH_MAX))
	{
		ft_putstr_fd(cwd, 1);
		ft_putstr_fd("\n", 1);
	}
	else
	{
		ft_putstr_fd("pwd: ", 2);
		ft_putstr_fd(strerror(errno), 2);
	}
}