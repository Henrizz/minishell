/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 15:45:39 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/09/10 17:47:50 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_input_heredoc(t_input **command, t_global *global)
{
	t_heredoc	here;
	t_input		*current;

	count_heredocs(command, global);
	current = (*command);
	if (process_heredocs(current, &here, global) == 1)
		return (1);
	return (0);
}

int	process_heredocs(t_input *current, t_heredoc *he, t_global *gl)
{
	int			i;
	int			j;

	j = 0;
	while (current)
	{
		i = 0;
		while (current->heredoc[i])
		{
			gl->filenames[j] = make_heredoc_filename(&current, i, gl);
			if (!gl->filenames[j])
				return (error_return("error making here_doc directory"));
			he->fd = open(gl->filenames[j], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (he->fd == -1)
				return (error_return("error making here_doc"));
			if (terminal_loop(he, current->heredoc[i++], gl) == 1)
				return (1);
			free(he->exp);
			close(he->fd);
			j++;
		}
		current = current->next;
	}
	gl->filenames[j] = NULL;
	return (0);
}

void	count_heredocs(t_input **command, t_global *global)
{
	int			i;
	int			count;
	t_input		*current;

	count = 0;
	current = (*command);
	while (current)
	{
		i = 0;
		while (current->heredoc[i])
		{
			i++;
			count++;
		}
		current = current->next;
	}
	global->filenames = malloc((count + 1) * sizeof(char *));
	if (!global->filenames)
		return ;
	global->filenames[0] = NULL;
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
