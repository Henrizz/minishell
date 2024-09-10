/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:46:25 by smanriqu          #+#    #+#             */
/*   Updated: 2024/09/10 19:38:37 by hzimmerm         ###   ########.fr       */
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

char	*change_directory(char *path, t_global *global)
{
	char	old_pwd[PATH_MAX];
	char	*temp;

	getcwd(old_pwd, PATH_MAX);
	if (ft_strncmp(path, "-", 1) == 0)
		temp = handle_old_pwd(global);
	else
		temp = ft_strdup(path);
	if (temp == NULL)
		return (NULL);
	if (!chdir(temp))
	{
		update_pwd_and_env(old_pwd, global->env_list, &global->env);
		global->exit_status = 0;
	}
	else
	{
		print_error(temp);
		global->exit_status = 1;
	}
	return (temp);
}

static void	handle_home_cd(t_global *global)
{
	char	*temp_home;
	char	*home;

	temp_home = get_env_value("HOME", global->env_list, 0);
	if (temp_home[0] == '\0')
	{
		handle_cd_error(global, "HOME not set");
		free(temp_home);
		return ;
	}
	home = change_directory(temp_home, global);
	free(home);
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
	char	*new;

	path = handle_cd_input(words, global);
	if (!path)
		return ;
	new = change_directory(path, global);
	free(path);
	if (!new)
		return ;
	free(new);
}
