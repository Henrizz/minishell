/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 16:04:14 by smanriqu          #+#    #+#             */
/*   Updated: 2024/09/10 20:04:37 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_error(char *path)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
}

void	handle_cd_error(t_global *global, char *message)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
	global->exit_status = 1;
}

char	*handle_old_pwd(t_global *global)
{
	char	*temp_oldpwd;

	temp_oldpwd = get_env_value("OLDPWD", global->env_list, 0);
	if (temp_oldpwd[0] == '\0')
	{
		handle_cd_error(global, "OLDPWD not set");
		free(temp_oldpwd);
		return (NULL);
	}
	return (temp_oldpwd);
}

char	*check_dashes(t_global *global, char *path)
{
	char	*temp;

	if (path[0] == '-' && path[1] == '\0')
	{
		temp = handle_old_pwd(global);
		printf("%s\n", temp);
	}
	else if (path[0] == '-' && path[1] == '-' && path[2] == '\0')
		temp = handle_old_pwd(global);
	else if ((path[0] == '-' && path[1] == '-' && path[2] != '\0') 
		|| (path[0] == '-' && path[1] != '-'))
	{
		ft_putstr_fd("minishell: cd: -", 2);
		ft_putchar_fd(path[1], 2);
		ft_putstr_fd(": invalid option\n", 2);
		global->exit_status = 2;
		return (NULL);
	}
	else
		temp = ft_strdup(path);
	return (temp);
}
