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

void	change_directory(char *path, t_global *global)
{
	char	old_pwd[PATH_MAX];
	char	*temp_oldpwd;

	getcwd(old_pwd, PATH_MAX);
	if(ft_strncmp(path, "-", 1) == 0)
	{
		temp_oldpwd = get_env_value("OLDPWD", global->env_list);
		if(temp_oldpwd[0] == '\0')
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			global->exit_status = 1;
			return;
		}
		path = ft_strdup(temp_oldpwd);
		free(temp_oldpwd);
	}
	if(path[0] == '\0' && global->home_expanded == 1)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		global->exit_status = 1;
		return;
	}
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

	if (!words[1])
	{
		path = get_env_value("HOME", global->env_list);
		if (path[0] == '\0')
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			global->exit_status = 1;
			free(path);
			return;
		}
	}
	else if (words[1] && !words[2])
	{
		path = ft_strdup(words[1]);
		if (!path)
		{
			ft_putstr_fd("minishell: cd: malloc error\n", 2);
			global->exit_status = 1;
			return;
		}
	}
	else
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		global->exit_status = 1;
		return;
	}
	change_directory(path, global);
	if (words[1] && !words[2])
    	free(path);
}

