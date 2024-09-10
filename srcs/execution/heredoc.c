/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 15:45:39 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/09/10 15:30:44 by hzimmerm         ###   ########.fr       */
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
			if (!here.filepath)
				return (error_return("error making here_doc directory"));
			here.fd = open(here.filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			free(here.filepath);
			if (here.fd == -1)
				return (error_return("error making here_doc"));
			if (terminal_loop(&here, current->heredoc[i++], global) == 1)
				return (1);
			free(here.exp);
			close(here.fd);
		}
		current = current->next;
	}
	return (0);
}

int	terminal_loop(t_heredoc *here, char *filename, t_global *global)
{
	here_expand(here, filename);
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

int	remove_heredocs(t_input **command, t_global *global)
{
	int		i;
	char	*filepath;
	t_input	*current;

	current = (*command);
	while (current)
	{
		i = 0;
		while (current->heredoc[i])
		{
			filepath = make_heredoc_filename(&current, i, global);
			if (!filepath)
				return (error_return("error making here_doc directory"));
			if (unlink(filepath) != 0)
			{
				free(filepath);
				return (error_return("error deleting here_doc"));
			}
			free(filepath);
			i++;
		}
		current = current->next;
	}
	return (0);
}
