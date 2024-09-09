/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 22:38:51 by Henriette         #+#    #+#             */
/*   Updated: 2024/09/09 18:04:09 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	create_pipes(t_pipe *exec)
{
	int	i;

	i = 0;
	if (i == exec->pipe_qty)
		return (0);
	while (i < exec->pipe_qty)
	{
		exec->pipe_fd[i] = malloc(2 * sizeof(int));
		if (exec->pipe_fd[i] == NULL)
			error_return("malloc error");
		if (pipe(exec->pipe_fd[i]) == -1)
			error_return("pipe error");
		i++;
	}
	return (0);
}

int	replace_pipes(t_input *command, t_pipe *exec)
{
	if (exec->pipe_qty == 0)
		return (0);
	if (command->cmd_ind == 0)
	{
		if ((!no_redirect(command, RED_OUT) && !no_redirect(command, APP_OUT))
			&& dup2(exec->pipe_fd[command->cmd_ind][1], 1) == -1)
			error_return("dup2 pipe[current][1]");
	}
	else if (command->cmd_ind == exec->pipe_qty)
	{
		if ((!no_redirect(command, RED_IN) && !command->heredoc[0])
			&& dup2(exec->pipe_fd[command->cmd_ind - 1][0], 0) == -1)
			error_return("dup2 pipe[previous][0]");
	}
	else
	{
		if ((!no_redirect(command, RED_IN) && !command->heredoc[0]) 
			&& dup2(exec->pipe_fd[command->cmd_ind - 1][0], 0) == -1)
			error_return("dup2 pipe[previous][0]");
		if ((!no_redirect(command, RED_OUT) && !no_redirect(command, APP_OUT))
			&& dup2(exec->pipe_fd[command->cmd_ind][1], 1) == -1)
			error_return("dup2 pipe[current][1]");
	}
	close_all_pipes(exec);
	return (0);
}

int	no_redirect(t_input *command, int flag)
{
	int	i;

	i = 0;
	while (command->redirections[i])
	{
		if (command->types[i] == flag)
			return (1);
		i++;
	}
	return (0);
}

void	close_all_pipes(t_pipe *exec)
{
	int	i;

	i = 0;
	if (i == exec->pipe_qty)
		return ;
	while (i < exec->pipe_qty)
	{
		if (exec->pipe_fd[i][0] != -1)
			close(exec->pipe_fd[i][0]);
		if (exec->pipe_fd[i][1] != -1)
			close(exec->pipe_fd[i][1]);
		free(exec->pipe_fd[i]);
		i++;
	}
	free(exec->pipe_fd);
}

void	wait_loop(t_input **command, t_global *global)
{
	t_input	*current;
	int		status;

	current = *command;
	while (current)
	{
		waitpid(current->pid, &status, 0);
		if (WIFEXITED(status))
			global->exit_status = WEXITSTATUS(status);
		current = current->next;
	}
}
