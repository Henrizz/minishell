/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 10:57:44 by Henriette         #+#    #+#             */
/*   Updated: 2024/09/09 15:17:29 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute(t_input **command, t_global *global)
{
	
	//int		stdin_copy;
	//int		stdout_copy;
	int	i;

	i = 0;
	if (save_in_out(&(global->stdin_copy), &(global->stdout_copy)) == -1 
		|| get_input_heredoc(command, global) == 1)
		return ;
	sig_execution();
	while ((*command)->words[i] && (*command)->words[i][0] == '\0' && (*command)->exp_word[i] == 1)
		i++;
	if (!(*command)->words[i] && !(*command)->next)
		return ;
	if (!(*command)->next && is_builtin((*command)->words + i))
	{
		if (make_redirection(command, global) == 1)
		{
			restore_in_out(&(global->stdin_copy), &(global->stdout_copy));
			return ;
		}
		else 
		{
			what_builtin((*command)->words + i, global, command);
			restore_in_out(&(global->stdin_copy), &(global->stdout_copy));
		}
	}
	else
		setup_and_run(command, global->exec, global);
	//restore_in_out(&stdin_copy, &stdout_copy);
}

int	setup_and_run(t_input **command, t_pipe *exec, t_global *global)
{
	t_input	*curr;

	curr = *command;
	get_cmd_index(command, exec);
	create_pipes(exec);
	while (curr)
	{
		curr->pid = fork();
		if (curr->pid == -1)
			return (error_return("fork error"));
		if (curr->pid == 0)
		{
			if (make_redirection(&curr, global) != 1 && curr->words[0])
				child_exec(curr, exec, global, command);
			else
			{
				restore_in_out(&(global->stdin_copy), &(global->stdout_copy));
				close_all_pipes(exec);
				cleanup_and_exit(global);
			}
		}
		curr = curr->next;
	}
	close_all_pipes(exec);
	wait_loop(command, global);
	return (0);
}

int	child_exec(t_input *curr, t_pipe *exec, t_global *glob, t_input **inpt)
{
	char	*cmd_file;
	int	i;

	i = 0;
	replace_pipes(curr, exec);
	close_all_pipes(exec);
	while (curr->words[i] && curr->words[i][0] == '\0' && curr->exp_word[i] == 1)
		i++;
	if (!curr->words[i])
	{
		free_command(inpt);
		cleanup_and_exit(glob);
	}	
	if (is_builtin(curr->words + i))
	{
		what_builtin(curr->words + i, glob, &curr);
		free_command(inpt);
		cleanup_and_exit(glob);
	}
	if (ft_strrchr(curr->words[i], '/'))
		cmd_file = prepare_path_command(curr->words[i], glob, inpt);
	else
		cmd_file = prepare_bare_cmd(curr->words, glob, inpt, i);
	cleanup(glob);
	execve(cmd_file, curr->words + i, glob->env);
	glob->exit_status = error_return("execve fail\n");
	free_command(inpt);
	cleanup_and_exit(glob);
	return (0);
}

char	*prepare_bare_cmd(char **cmd, t_global *glob, t_input **inpt, int i)
{
	char	*cmd_file;

	if (cmd[i][0] == '\0' && (*inpt)->exp_word[i] == 0)
	{
		ft_putstr_fd(": command not found\n", 2);
		cmd_file = NULL;
	}
	else
		cmd_file = find_cmd_file(cmd + i, glob->env);
	if (cmd_file == NULL)
	{
		free_command(inpt);
		//restore_in_out(stdin_copy, stdout_copy);
		//close(*stdin_copy);
		//close(*stdout_copy);
		glob->exit_status = 127;
		cleanup_and_exit(glob);
	}
	return (cmd_file);
}

char	*prepare_path_command(char *word, t_global *global, t_input **input)
{
	char	*file;

	file = ft_strdup(word);
	if (!file || access(file, F_OK) != 0)
		file_error(file, ": No such file or directory\n", global, input);
	if (is_directory(file))
		file_error(file, ": Is a directory\n", global, input);
	if (access(file, X_OK) != 0)
		file_error(file, ": Permission denied\n", global, input);
	return (file);
}
