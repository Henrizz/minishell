/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 16:55:54 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/08/15 17:25:28 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	make_redirections(t_input **command, char *pwd)
{
	if (redirect_in_out(command) == 1)
	{
		(*command)->exit_status = 1;
		return (1);
	}
	if (redirect_heredoc(command, pwd) == 1)
	{
		(*command)->exit_status = 1;
		return (1);
	}
	if (redirect_append(command) == 1)
	{
		(*command)->exit_status = 1;
		return (1);
	}
	return (0);
}

int	redirect_heredoc(t_input **command, char *pwd)
{
	int	i;
	int fd;
	char *filepath;

	
	if ((*command)->heredoc)
	{
		i = 0;
		while ((*command)->heredoc[i])
		{
			filepath = make_heredoc_filename(command, i, pwd);
			fd = open(filepath, O_RDONLY);
			if (fd == -1)
				return (error_return((*command)->heredoc[i]));
			if (dup2(fd, 0) == -1)
			{
				// need to close copy of stdout?
				close(fd);
				return (error_return("dup2"));
			}
			close(fd);
			i++;
		}
	}
	return (0);
}

int	redirect_append(t_input **command)
{
	int	i;
	int fd;

	i = 0;
	while ((*command)->app_out[i])
	{
		fd = open((*command)->app_out[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
			return (error_return((*command)->app_out[i]));
		if (dup2(fd, 1) == -1)
		{
			// need to close copy of stdout?
			close(fd);
			return (error_return("dup2"));
		}
		close(fd);
		i++;
	}
	return (0);
}

int	redirect_in_out(t_input **command)
{
	int	i;
	if ((*command)->red_in)
	{
		i = 0;
		while ((*command)->red_in[i])
		{
			if (redirection_in(command, i) == 1)
				return (1);
			i++;
		}
	}
	if ((*command)->red_out)
	{
		i = 0;
		while ((*command)->red_out[i])
		{
			if (redirection_out(command, i) == 1)
				return (1);
			i++;
		}
	}
	return (0);
}

/* separated function for shortening the redirection while loop */
int	redirection_out(t_input **command, int i)
{
	int	fd;
	
	fd = open((*command)->red_out[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (error_return((*command)->red_out[i]));
	if (dup2(fd, 1) == -1)
	{
		// need to close copy of stdout?
		close(fd);
		return (error_return("dup2"));
	}
	close(fd);
	return (0);
}

/* separated function for shortening the redirection while loop */
int	redirection_in(t_input **command, int i)
{
	int	fd;
	
	if (access((*command)->red_in[i], R_OK) == -1)
		return (error_return((*command)->red_in[i]));
	fd = open((*command)->red_in[i], O_RDONLY);
	if (fd == -1)
		return (error_return("open"));
	if (dup2(fd, 0) == -1)
	{
		// need to close copy of stdin?
		close(fd);
		return (error_return("dup2"));
	}
	close(fd);
	return (0);
}
