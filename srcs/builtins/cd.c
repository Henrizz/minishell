/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:46:25 by smanriqu          #+#    #+#             */
/*   Updated: 2024/09/10 14:38:31 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*handle_special_cases(char *path, t_global *global)
{
	if (ft_strncmp(path, "-", 1) == 0)
	{
		if (path[1] == '\0')
		{
			path = handle_old_pwd(global);
			if (path != NULL)
				printf("%s\n", path);
		}
		else if (path[1] == '-' && path[2] == '\0')
			path = handle_old_pwd(global);
		else if ((path[1] == '-' && path[2] != '\0') || (path[1] != '-'))
		{
			ft_putstr_fd("minishell: cd: -", 2);
			ft_putchar_fd(path[1], 2);
			ft_putstr_fd(": invalid option\n", 2);
			global->exit_status = 2;
			return (NULL);
		}
	}
	return (path);
}

static void	change_directory(char *path, t_global *global)
{
	char	old_pwd[PATH_MAX];

	if (getcwd(old_pwd, PATH_MAX) == NULL)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		global->exit_status = 1;
		return ;
	}
	path = handle_special_cases(path, global);
	if (path == NULL)
		return ;
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

	temp_home = get_env_value("HOME", global->env_list, 0);
	if (temp_home[0] == '\0')
	{
		handle_cd_error(global, "HOME not set");
		free(temp_home);
		temp_home = NULL;
		return ;
	}
	change_directory(temp_home, global);
	free(temp_home);
	temp_home = NULL;
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
			path = NULL;
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
	path = NULL;
}
