#include "../../includes/minishell.h"

//char *getcwd(char *buffer, size_t size);

void	pwd(t_global *global)
{
	char cwd[PATH_MAX];

	if(getcwd(cwd, PATH_MAX))
	{
		ft_putstr_fd(cwd, 1);
		ft_putstr_fd("\n", 1);
		global->exit_status = 0;
	}
	else
	{
		ft_putstr_fd("pwd: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		global->exit_status = 1;
	}
}