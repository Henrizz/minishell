/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 10:57:44 by Henriette         #+#    #+#             */
/*   Updated: 2024/08/09 17:26:38 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void execute(t_input **command, t_env *env_list, char **env, char *pwd)
{
	t_pipe	*exec;
	int	stdin_copy;
	int	stdout_copy;
	
	exec = NULL;
	if (save_in_out(&stdin_copy, &stdout_copy) == -1) //save stdin and stdout to restore later
		return;
	// here get heredoc input, from all heredocs and save fds in separate array (or replace heredoc array)
	if (get_input_heredoc(command, env, pwd) == -1)
		return;
	if (!(*command)->next && is_builtin(command)) //this means if there is only one command (so no pipe) and it's a builtin
	{
		if (make_redirections(command) == -1)
			return;
		what_builtin((*command)->words, env_list);
	}	
	else
	{
		//setup_and_run(command, exec, env);
		simple_set_up_and_run_processes(command, env); //for testing
	}	
	restore_in_out(&stdin_copy, &stdout_copy); //restore stdin and stdout
	// here remove heredocs, if any
}

int simple_set_up_and_run_processes(t_input **command, char **env) //simple command without pipe layout
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
			/*temp = ft_strrchr((*command)->words[0], '/') + 1;
			free((*command)->words[0]);
			(*command)->words[0] = ft_strdup(temp);
			if ((*command)->words[0] == NULL)
				error_return("minishell: error ft_strdup");*/
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
}

