/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stephaniemanrique <stephaniemanrique@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 10:57:44 by Henriette         #+#    #+#             */
/*   Updated: 2024/08/16 17:15:32 by stephaniema      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void execute(t_input **command, t_global *global)
{
	t_pipe	*exec;
	int	stdin_copy;
	int	stdout_copy;

	exec = malloc(sizeof(t_pipe));
	if (!exec)
		return;
	if (save_in_out(&stdin_copy, &stdout_copy) == -1 || get_input_heredoc(command, global->env, global->pwd) == -1)
	{
		free(exec);
		return;
	}
	if (!(*command)->next && is_builtin(command))
	{
		if (make_redirections(command, global->pwd) == 1)
		{
			free(exec);
			return;
		}
		what_builtin((*command)->words, global);
	}
	else
		setup_and_run(command, exec, global);
	free(exec);
	restore_in_out(&stdin_copy, &stdout_copy);
}

int setup_and_run(t_input **command, t_pipe *exec, t_global *global)
{
	t_input *current;
	//int	i;

	current = *command;
	//i = 0;
	get_cmd_index(command, exec);
	create_pipes(exec);
	while (current)
	{
		current->pid = fork();
		if (current->pid == -1)
			return (error_return("fork error"));
		if (current->pid == 0)
		{
			if (make_redirections(&current, global->pwd) != 1 && current->words[0])
				child_process_exec(current, exec, global);
			else
				exit(EXIT_SUCCESS);
		}
		current = current->next;
	}
	close_all_pipes(exec);
	wait_loop(command);
	return (0);
}

int	child_process_exec(t_input *command, t_pipe *exec, t_global *global)
{
	char	*cmd_file;
	//char	**cmd;

	cmd_file = NULL;
	//cmd = NULL;
	if (is_builtin(&command))
	{
		what_builtin(command->words, global);
		exit(EXIT_SUCCESS);
	}
	replace_pipes(command, exec);
	close_all_pipes(exec);
	if (ft_strrchr(command->words[0], '/'))
	{
		cmd_file = ft_strdup(command->words[0]);
		if (cmd_file == NULL)
			error_return("error duplicating command->words[0]");
		if (access(cmd_file, X_OK) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd_file, 2);
			ft_putstr_fd(": command not found\n", 2);
			free(cmd_file);
			exit(127);
		}
	}
	else
	{
		cmd_file = find_cmd_file(command->words, global->env);
		if (cmd_file == NULL)
		{
			exit(127);
		}
	}
	execve(cmd_file, command->words, global->env);
	ft_putstr_fd("execve fail\n", 2);
	exit(EXIT_FAILURE);
}
