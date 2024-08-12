#include "../../includes/minishell.h"

//int chdir(const char *path);


void	cd(char *path, t_env *env_list)
{
	char old_pwd[PATH_MAX];
	char pwd[PATH_MAX];
	//TODO too many arguments
	//TODO exit status?
	getcwd(old_pwd, PATH_MAX);
	if(!chdir(path))
	{
		getcwd(pwd, PATH_MAX);
		set_env("OLDPWD", old_pwd, env_list, 1);
		set_env("PWD", pwd, env_list, 1);
		return;
	}
	else
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		printf("\n");
	}
}

