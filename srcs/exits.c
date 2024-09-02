/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 15:13:31 by Henriette         #+#    #+#             */
/*   Updated: 2024/09/02 12:56:21 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exit_shell(int exit_status)
{
	//if (exit_status == EXIT_SUCCESS && message != NULL)
		//ft_putstr_fd(message, 2);
	//if (exit_status == EXIT_FAILURE && message != NULL)
		//perror(message);
	rl_clear_history();
	exit(exit_status);
}

void	cleanup_and_exit(t_global *global)
{
	int exit_status = global->exit_status;
	
	close(global->history_fd);
	remove_heredoc(global->env, global->pwd, global->exit_status);
	rl_clear_history();
	free_env_list(&global->env_list);
	free_array(global->env);
	free(global->prompt);
	free(global);
	exit(exit_status);
}

void	shell_exit(t_global *global)
{
	ft_putstr_fd("exit\n", 1);
	cleanup_and_exit(global);
}

void	free_command(t_input **command)
{
	t_input	*temp;

	temp = *command;
	while (*command != NULL)
	{
		temp = *command;
		*command = (*command)->next;
		free_array(temp->words);
		free_array(temp->redirections);
		free_array(temp->heredoc);
		free(temp->types);
		free(temp);
	}
	*command = NULL;
}

/* to free a twodimensional array of strings */
void	free_array(char **str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return ;
	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str);
}

int	error_return(char *message)
{
	ft_putstr_fd("minishell: ", 2);
	perror(message);
	return (1);
}
