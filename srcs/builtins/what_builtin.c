/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   what_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 15:44:26 by smanriqu          #+#    #+#             */
/*   Updated: 2024/09/04 15:45:22 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	what_builtin(char **command_words, t_global *global, t_input **command)
{
	if (!command_words[0])
		return (0);
	if (!ft_strncmp(command_words[0], "echo", 4))
		echo(command_words, global);
	if (!ft_strncmp(command_words[0], "cd", 2))
		cd(command_words, global);
	if (!ft_strncmp(command_words[0], "pwd", 3))
		pwd(global);
	if (!ft_strncmp(command_words[0], "export", 6))
		export(command_words, global);
	if (!ft_strncmp(command_words[0], "unset", 5))
		unset(command_words, global);
	if (!ft_strncmp(command_words[0], "env", 3))
		env_cmd(command_words, global);
	if (!ft_strncmp(command_words[0], "exit", 4))
		exit_cmd(command_words, global, command);
	return (global->exit_status);
}
