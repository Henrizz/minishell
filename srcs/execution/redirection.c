/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Henriette <Henriette@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 16:55:54 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/08/21 21:45:14 by Henriette        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	make_redirections(t_input **command, t_global *global)
{
	int 	i;

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
			return (1);
		i++;
	}
	if (redirect_heredoc(command, global) == 1)
	{
		global->exit_status = 1;
		return (1);
	}
	return (0);
}


/*int	make_redirections(t_input **command, t_global *global)
{
	if (redirect_in_out(command) == 1)
	{
		global->exit_status = 1;
		return (1);
	}
	if (redirect_heredoc(command, global) == 1)
	{
		global->exit_status = 1;
		return (1);
	}
	if (redirect_append(command) == 1)
	{
		global->exit_status = 1;
		return (1);
	}
	return (0);
}*/

int	redirect_heredoc(t_input **command, t_global *global)
{
	int	i;
	int fd;
	char *filepath;

	
	if ((*command)->heredoc)
	{
		i = 0;
		while ((*command)->heredoc[i])
		{
			filepath = make_heredoc_filename(command, i, global);
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

int	redirect_append(char *filename, t_global *global)
{
	int fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		global->exit_status = 1;
		return (error_return(filename));
	}
	if (dup2(fd, 1) == -1)
	{
		// need to close copy of stdout?
		global->exit_status = 1;
		close(fd);
		return (error_return("dup2"));
	}
	close(fd);
	return (0);
}

/*int	redirect_in_out(t_input **command)
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
}*/

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
		// need to close copy of stdout?
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
		// need to close copy of stdin?
		close(fd);
		global->exit_status = 1;
		return (error_return("dup2"));
	}
	close(fd);
	return (0);
}
