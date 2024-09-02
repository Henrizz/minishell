/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 16:55:54 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/09/02 18:41:24 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	make_redirection(t_input **command, t_global *global, t_input **input)
{
	int	i;

	i = 0;
	while ((*command)->redirections[i])
	{
		if ((*command)->types[i] == RED_IN)
			redirection_in((*command)->redirections[i], global);
		if ((*command)->types[i] == RED_OUT)
			redirection_out((*command)->redirections[i], global);
		if ((*command)->types[i] == APP_OUT)
			redirect_append((*command)->redirections[i], global);
		if (global->exit_status == 1)
		{
			free_command(input);
			return (1);
		}
		i++;
	}
	if (redirect_heredoc(command, global) == 1)
	{
		global->exit_status = 1;
		return (1);
	}
	return (0);
}

int	redirect_heredoc(t_input **command, t_global *global)
{
	int		i;
	int		fd;
	char	*filepath;

	if ((*command)->heredoc)
	{
		i = 0;
		while ((*command)->heredoc[i])
		{
			filepath = make_heredoc_filename(command, i, global);
			fd = open(filepath, O_RDONLY);
			free(filepath);
			if (fd == -1)
				return (error_return((*command)->heredoc[i]));
			if (dup2(fd, 0) == -1)
			{
				close(fd);
				return (error_return("dup2"));
			}
			close(fd);
			i++;
		}
	}
	return (0);
}

int	redirect_append(char *filename, t_global *global)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		global->exit_status = 1;
		return (error_return(filename));
	}
	if (dup2(fd, 1) == -1)
	{
		global->exit_status = 1;
		close(fd);
		return (error_return("dup2"));
	}
	close(fd);
	return (0);
}

/* separated function for shortening the redirection while loop */
int	redirection_out(char *filename, t_global *global)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		global->exit_status = 1;
		return (error_return(filename));
	}
	if (dup2(fd, 1) == -1)
	{
		global->exit_status = 1;
		close(fd);
		return (error_return("dup2"));
	}
	close(fd);
	return (0);
}

/* separated function for shortening the redirection while loop */
int	redirection_in(char *filename, t_global *global)
{
	int	fd;

	if (access(filename, R_OK) == -1)
	{
		global->exit_status = 1;
		return (error_return(filename));
	}
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		global->exit_status = 1;
		return (error_return("open"));
	}
	if (dup2(fd, 0) == -1)
	{
		close(fd);
		global->exit_status = 1;
		return (error_return("dup2"));
	}
	close(fd);
	return (0);
}
