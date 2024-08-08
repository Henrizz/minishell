/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 10:57:44 by Henriette         #+#    #+#             */
/*   Updated: 2024/08/08 16:51:30 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void execute(t_input **command, t_env *env_list, char **env)
{
	int	pipe_qty;
	
	(void)env_list;
	pipe_qty = set_up_pipes_redirections(command);
	if (pipe_qty == -1)
		return;
	// later make more complex execution setup with various amount of child processes, this is only to test for one simple command
	if (!(*command)->next && is_builtin(command)) //this means if there is only one command (so no pipe) and it's a builtin
		what_builtin((*command)->words, env_list);		
	else
		set_up_and_run_processes(command, env);
}

int set_up_and_run_processes(t_input **command, char **env)
{
	int	pid;
	char	*cmd_file;
	char *temp;

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
			temp = ft_strrchr((*command)->words[0], '/') + 1;
			free((*command)->words[0]);
			(*command)->words[0] = ft_strdup(temp);
			if ((*command)->words[0] == NULL)
				error_return("minishell: error ft_strdup");
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
		printf("execve fail\n");
		return (-1);
	}
	else 
		waitpid(pid, NULL, 0);
	return (0);
}

int set_up_pipes_redirections(t_input **command)
{
	int	pipe_qty;

	pipe_qty = get_cmd_index(command);
	//printf("%d\n", pipe_qty);
	
	//set up pipes and redirections - if error, return -1
	return (pipe_qty);
}

