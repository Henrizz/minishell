/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 15:13:31 by Henriette         #+#    #+#             */
/*   Updated: 2024/09/09 15:18:02 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cleanup_and_exit(t_global *global)
{
	int	exit_status;

	exit_status = global->exit_status;
	close(global->history_fd);
	remove_heredoc(global->env, global->pwd, global->exit_status);
	rl_clear_history();
	free_env_list(&global->env_list);
	free_array(global->env);
	free(global->prompt);
	free(global->exec);
	if (global->stdin_copy != -1)
	{
		close(global->stdin_copy);
		global->stdin_copy = -1;
	}
	if (global->stdout_copy != -1)
	{
		close(global->stdout_copy);
		global->stdout_copy = -1;
	}
	free(global);
	exit(exit_status);
}

void	cleanup(t_global *global)
{
	
	close(global->history_fd);
	free_env_list(&global->env_list);
	free(global->prompt);
	free(global->exec);
	//close(global->stdin_copy);
	//close(global->stdout_copy);
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
		free(temp->exp_word);
		free(temp->exp_redir);
		free(temp->types);
		free(temp);
	}
	
	*command = NULL;
}

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
	char	*line;

	line = ft_strjoin("minishell: ", message);
	if (!line)
	{
		ft_putstr_fd("error allocating perror line", 2);
		return (1);
	}
	perror(line);
	free(line);
	return (1);
}
