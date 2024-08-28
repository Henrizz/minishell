/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 10:57:44 by Henriette         #+#    #+#             */
/*   Updated: 2024/08/28 18:45:24 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute(t_input **command, t_global *global)
{
	t_pipe	*exec;
	int		stdin_copy;
	int		stdout_copy;

	exec = malloc(sizeof(t_pipe));
	if (!exec)
		return ;
	if (save_in_out(&stdin_copy, &stdout_copy) == -1 
		|| get_input_heredoc(command, global) == 1)
		return (free(exec));
	sig_execution();
	if (!(*command)->next && is_builtin(command))
	{
		if (make_redirections(command, global) == 1)
		{
			restore_in_out(&stdin_copy, &stdout_copy);
			free(exec);
			return ;
		}
		else 
			what_builtin((*command)->words, global);
	}
	else
		setup_and_run(command, exec, global);
	free(exec);
	restore_in_out(&stdin_copy, &stdout_copy);
}

int	setup_and_run(t_input **command, t_pipe *exec, t_global *global)
{
	t_input	*current;

	current = *command;
	get_cmd_index(command, exec);
	create_pipes(exec);
	while (current)
	{
		current->pid = fork();
		if (current->pid == -1)
			return (error_return("fork error"));
		if (current->pid == 0)
		{
			if (make_redirections(&current, global) != 1 && current->words[0])
				child_process_exec(current, exec, global);
			else
				exit(global->exit_status);
		}
		current = current->next;
	}
	close_all_pipes(exec);
	wait_loop(command, global);
	return (0);
}

int	child_process_exec(t_input *command, t_pipe *exec, t_global *global)
{
	char	*cmd_file;
	int	i;

	i = 0;
	replace_pipes(command, exec);
	close_all_pipes(exec);
	if (is_builtin(&command))
		exit(what_builtin(command->words, global));
	while (command->words[i][0] == '\0')
		i++;
	if (ft_strrchr(command->words[i], '/'))
	{
		cmd_file = ft_strdup(command->words[i]);
		if (!cmd_file || access(cmd_file, F_OK) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd_file, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			free(cmd_file);
			exit(127);
		}
		if (is_directory(cmd_file))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd_file, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			free(cmd_file);
			exit(126);
		}
		if (access(cmd_file, X_OK) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd_file, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			free(cmd_file);
			exit(126);
		}
	}
	else
	{
		cmd_file = find_cmd_file(command->words + i, global->env);
		if (cmd_file == NULL)
			exit(127);
	}
	execve(cmd_file, command->words + i, global->env);
	exit(error_return("execve fail\n"));
}

int	is_directory(char *name)
{
	DIR 	*dir;
	
	dir = opendir(name);
	if (dir)
	{
		closedir(dir);
		return (1);
	}
	return (0);
}
