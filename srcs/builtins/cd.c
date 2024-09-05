/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:46:25 by smanriqu          #+#    #+#             */
/*   Updated: 2024/09/05 13:35:09 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	update_pwd_and_env(char *old_pwd, t_env *env_list, char ***env)
{
	char	pwd[PATH_MAX];

	getcwd(pwd, PATH_MAX);
	set_env("OLDPWD", old_pwd, env_list, 1);
	set_env("PWD", pwd, env_list, 1);
	set_env_array(env_list, env);
}

static void	change_directory(char *path, t_global *global)
{
	char	old_pwd[PATH_MAX];

	getcwd(old_pwd, PATH_MAX);
	if (ft_strncmp(path, "-", 1) == 0)
	{
		path = handle_old_pwd(global);
		if (path == NULL)
			return ;
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

static void	handle_home_cd(t_global *global)
{
	char	*temp_home;

	temp_home = get_env_value("HOME", global->env_list);
	if (temp_home[0] == '\0')
	{
		handle_cd_error(global, "HOME not set");
		free(temp_home);
		return ;
	}
	change_directory(temp_home, global);
	free(temp_home);
}

char	*handle_cd_input(char **words, t_global *global)
{
	char	*path;

	if (!words[1])
	{
		handle_home_cd(global);
		return (NULL);
	}
	else if (words[1] && !words[2])
	{
		path = ft_strdup(words[1]);
		if (path[0] == '\0' && global->home_expanded == 1)
		{
			handle_cd_error(global, "HOME not set");
			free(path);
			return (NULL);
		}
		return (path);
	}
	else
	{
		handle_cd_error(global, "too many arguments");
		return (NULL);
	}
}

void	cd(char **words, t_global *global)
{
	char	*path;

	path = handle_cd_input(words, global);
	if (!path)
		return ;
	change_directory(path, global);
	free(path);
}
