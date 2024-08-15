/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 10:57:44 by Henriette         #+#    #+#             */
/*   Updated: 2024/08/15 14:51:12 by hzimmerm         ###   ########.fr       */
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
		if (make_redirections(command, global->pwd) == -1)
			return;
		what_builtin((*command)->words, global);
		free(exec);
	}
	else
	{
		setup_and_run(command, exec, global);
		free(exec);
		//simple_set_up_and_run_processes(command, global->env); //for testing
	}
	restore_in_out(&stdin_copy, &stdout_copy);
}

int setup_and_run(t_input **command, t_pipe *exec, t_global *global)
{
	int	pid;
	t_input *current;
	
	current = *command;
	get_cmd_index(command, exec);
	create_pipes(exec);
	while (current)
	{
		pid = fork();
		if (pid == -1)
			return (error_return("fork error"));
		if (pid == 0)
		{
			if (make_redirections(&current, global->pwd) != -1 && current->words[0])
				child_process_exec(current, exec, global);
			else
				exit(EXIT_SUCCESS);
		}
		current = current->next;
	}
	close_all_pipes(exec);
	wait_loop(exec);
	return (0);
}

int	child_process_exec(t_input *command, t_pipe *exec, t_global *global)
{
	char	*cmd_file;
	char	**cmd;

	cmd_file = NULL;
	cmd = NULL;
	//printf("command ind: %d\n", command->cmd_ind);
	if (is_builtin(&command))
	{
		what_builtin(command->words, global); //double check cases, for example cd after pipe should not execute? 
		//free_env_list(&global->env_list);
		//free(global);
		//close_all_pipes(exec);
		exit(EXIT_SUCCESS);
	}
	replace_pipes(command, exec);
	close_all_pipes(exec);
	if (ft_strrchr(command->words[0], '/'))
	{
		cmd_file = ft_strdup(command->words[0]);
		if (cmd_file == NULL)
			error_return("error ft_strdup");
		if (access(cmd_file, X_OK) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd_file, 2);
			ft_putstr_fd(": command not found\n", 2);
			return (0);
		}
	}
	else
	{
		cmd_file = find_cmd_file(command->words, global->env);
		if (cmd_file == NULL)
		{
			exit(EXIT_FAILURE);
		}
	}
	execve(cmd_file, command->words, global->env);
	ft_putstr_fd("execve fail\n", 2);
	exit(EXIT_FAILURE);
}

/*int simple_set_up_and_run_processes(t_input **command, char **env) //simple command without pipe layout
{
	int	pid;
	char	*cmd_file;
	//char *temp;

	cmd_file = NULL;
	pid = fork();
	if (pid == -1)
		return (error_return("minishell: fork error"));
	if (pid == 0)
	{
		if (ft_strrchr((*command)->words[0], '/'))
		{
			cmd_file = ft_strdup((*command)->words[0]);
			if (cmd_file == NULL)
				error_return("minishell: error ft_strdup");
			if (access(cmd_file, X_OK) != 0)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(cmd_file, 2);
				ft_putstr_fd(": command not found\n", 2);
				return (0);
			}
		}
		else
		{
			cmd_file = find_cmd_file((*command)->words, env);
			if (cmd_file == NULL)
			{
				free(cmd_file);
				exit(EXIT_FAILURE);
			}
		}
		execve(cmd_file, (*command)->words, env);
		//printf("execve fail\n"); --> make better error catch
		return (-1);
	}
	else
		waitpid(pid, NULL, 0);
	return (0);
}*/

