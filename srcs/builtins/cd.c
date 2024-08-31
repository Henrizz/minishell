#include "../../includes/minishell.h"

static void	print_error(char *path)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
}

static void	update_pwd_and_env(char *old_pwd, t_env *env_list, char ***env)
{
	char pwd[PATH_MAX];
	getcwd(pwd, PATH_MAX);
	set_env("OLDPWD", old_pwd, env_list, 1);
	set_env("PWD", pwd, env_list, 1);
	set_env_array(env_list, env);
}

char	*get_cd_path(char **words, t_global *global)
{
	if (!words[1])
	{
		char *path = get_env_value("HOME", global->env_list);
		if (!path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			global->exit_status = 1;
			return (NULL);
		}
		return (path);
	}
	return (ft_strdup(words[1]));
}

void	change_directory(char *path, t_global *global)
{
	char	old_pwd[PATH_MAX];

	getcwd(old_pwd, PATH_MAX);
	if (!chdir(path))
	{
		update_pwd_and_env(old_pwd, global->env_list, &global->env);
		global->exit_status = 0;
	}
	else
	{
		print_error(path);
		global->exit_status = 1;
	}
}

void	cd(char **words, t_global *global)
{
	char *path;
	if (words[2] != NULL)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		global->exit_status = 1;
		return;
	}
	path = get_cd_path(words, global);
	if (path)
	{
		change_directory(path, global);
		free(path);
	}
}

/*void	cd(char **words, t_global *global)
{
	char *path;
	char old_pwd[PATH_MAX];


	if (words[2] != NULL)
	{
		//printf("here");
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		global->exit_status = 1;
		return;
	}
	if(!words[1])
	{
		path = get_env_value("HOME", global->env_list);
		if(!path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			global->exit_status = 1;
			return;
		}
	}
	getcwd(old_pwd, PATH_MAX);
	path = ft_strdup(words[1]);
	if(!chdir(path))
	{
		update_pwd_and_env(old_pwd, global->env_list, &global->env);
		global->exit_status = 0;
	}
	else
	{
		print_error(path);
		global->exit_status = 1;
	}
}*/

