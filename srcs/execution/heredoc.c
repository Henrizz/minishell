/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 15:45:39 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/09/10 13:27:26 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_input_heredoc(t_input **command, t_global *global)
{
	int			i;
	t_heredoc	here;
	t_input		*current;

	current = (*command);
	while (current)
	{
		i = 0;
		while (current->heredoc[i])
		{
			here.filepath = make_heredoc_filename(&current, i, global);
			here.fd = open(here.filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (here.fd == -1 || !here.filepath)
				return (error_return("error making here_doc"));
			free(here.filepath);
			here_expand(&here, current->heredoc[i]);
			if (terminal_loop(&here, current->heredoc[i], global) == 1)
				return (1);
			i++;
			free(here.exp);
			close(here.fd);
		}
		current = current->next;
	}
	return (0);
}

int	terminal_loop(t_heredoc *here, char *filename, t_global *global)
{
	while (1)
	{
		here->line = readline("> ");
		if (here->line == NULL)
			return (print_eof_warning(here->count, here->exp), 0);
		else if (!ft_strncmp(here->line, here->exp, ft_strlen(filename)))
			return (free(here->line), 0);
		if (g_global_signum == SIGINT)
			return (free(here->line), free(here->exp), 1);
		if (here->flag == 0)
			here->temp = expanding_var(here->line, global, &here->none);
		else
			here->temp = ft_strdup(here->line);
		if (!here->temp)
			return (free(here->line), free(here->exp), 1);
		ft_putstr_fd(here->temp, here->fd);
		ft_putstr_fd("\n", here->fd);
		free(here->line);
		free(here->temp);
		here->count++;
	}
	signal(SIGINT, reset_line);
	//close(here->fd);
	return (0);
}

void	print_eof_warning(int count, char *here_exp)
{
	char	*mssg1;
	char	*mssg2;

	mssg1 = "minishell: warning: here-document delimited at line ";
	mssg2 = " by end-of-file (wanted `";
	printf("%s%d%s%s')\n", mssg1, count, mssg2, here_exp);
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
