/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 16:55:54 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/08/09 15:29:25 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* this is not right, was just a first draft, keeping it here in case I need pieces of it */
/*int set_up_pipes_redirections(t_input **command, t_pipe *exec)
{

	get_cmd_index(command, exec); //here also allocation of exec

	//per command, it needs to be first redirected, if files lack permission, it stops there, but next command is executed
	//redirection trumps pipe if there is both
	make_redirections(command);
	exec->pipe_fd = malloc((exec->pipe_qty) * sizeof(int *));
	//set up pipes and redirections - if error, return -1
	return (0);
}*/

int	make_redirections(t_input **command)
{
	if (redirect_in_out(command) == -1)
		return (-1);
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
			if (redirection_in(command, i) == -1)
				return (-1);
			i++;
		}
	}
	if ((*command)->red_out)
	{
		i = 0;
		while ((*command)->red_out[i])
		{
			if (redirection_out(command, i) == -1)
				return (-1);
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
