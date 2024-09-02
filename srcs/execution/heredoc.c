/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 15:45:39 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/09/02 13:02:15 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_input_heredoc(t_input **command, t_global *global)
{
	int			i;
	t_heredoc	here;
	t_input	*current;

	current = (*command);
	while (current)
	{
		i = 0;
		while (current->heredoc[i])
		{
			here.filepath = make_heredoc_filename(&current, i, global);
			here.fd = open(here.filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (here.fd == -1 || !here.filepath)
				return (error_return("error making here_doc file or directory"));
			free(here.filepath);
			here_expand(&here, current->heredoc[i]);
			if (terminal_loop(&here, current->heredoc[i], global) == 1)
				return (1);
			i++;
			free(here.expand);
		}
		current = current->next;
	}
	return (0);
}

int	terminal_loop(t_heredoc *here, char *filename, t_global *global)
{
	char	*mssg;

	mssg = "minishell: warning: here-document delimited at line by end-of-file (wanted `";
	while (1)
	{
		here->line = readline("> ");
		if (here->line == NULL)
			return (printf("%.52s%d%s%s')\n", mssg, here->count, mssg + 51, here->expand), 0);
		else if (!ft_strncmp(here->line, here->expand, ft_strlen(filename)))
			return (free(here->line), 0);
		if(global_signum == SIGINT)
		{
			//ft_printf("inside second if\n");
			free(here->line);
			//global_signum = 0;
			free(here->expand);
			return (1);
		}
		if (here->flag == 0)
			here->temp = expanding_var(here->line, global);
		else
			here->temp = ft_strdup(here->line);
		if (!here->temp)
			return (free(here->line), free(here->expand), 1);
		ft_putstr_fd(here->temp, here->fd);
		ft_putstr_fd("\n", here->fd);
		free(here->line);
		free(here->temp);
		here->count++;
	}
	signal(SIGINT, reset_line);
	return (0);
}


int	remove_heredoc(char **env, char *pwd, int exit_status)
{
	int		pid;
	char	*cmd[4];
	char	*cmd_file;

	cmd_file = NULL;
	cmd[0] = "rm";
	cmd[1] = "-rf";
	cmd[2] = ft_strjoin(pwd, "/.heredocs");
	if (!cmd[2])
		return (error_return("error allocating heredoc path"));
	cmd[3] = NULL;
	if (access(cmd[2], F_OK) == 0)
	{
		pid = fork();
		if (pid == -1)
			return (free(cmd[2]), error_return("fork error"));
		if (pid == 0)
		{
			cmd_file = find_cmd_file(cmd, env);
			execve(cmd_file, cmd, env);
			exit(error_return("execve fail\n"));
		}
		waitpid(pid, &exit_status, 0);
	}
	return (free(cmd[2]), 0);
}
