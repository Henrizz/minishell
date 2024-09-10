/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 16:55:54 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/09/10 13:20:29 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	make_redirection(t_input **command, t_global *global)
{
	int	i;

	i = 0;
	while ((*command)->redirections[i])
	{
		if (process_redir((*command)->redirections[i], 
				(*command)->types[i], global, command))
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

int	process_redir(char *redir, int type, t_global *global, t_input **cmd)
{
	if (type == RED_IN)
	{
		if (redirection_in(redir, global))
			return (1);
	}
	if (type == RED_OUT)
	{
		if (redirection_out(redir, global, cmd))
			return (1);
	}
	if (type == APP_OUT)
	{
		if (redirect_append(redir, global, cmd))
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

int	redirect_append(char *filename, t_global *global, t_input **command)
{
	int	fd;
	int	len;

	len = ft_strlen((*command)->words[0]);
	if (filename[0] == '$')
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": ambiguous redirect\n", 2);
		global->exit_status = 1;
		return (1);
	}
	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		global->exit_status = 1;
		return (error_return(filename));
	}
	if (ft_strncmp((*command)->words[0], "exit", len) && dup2(fd, 1) == -1)
	{
		global->exit_status = 1;
		close(fd);
		return (error_return("dup2"));
	}
	return (close(fd), 0);
}

int	redirection_out(char *filename, t_global *global, t_input **command)
{
	int	fd;
	int	len;

	len = ft_strlen((*command)->words[0]);
	if (filename[0] == '$')
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": ambiguous redirect\n", 2);
		global->exit_status = 1;
		return (1);
	}
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		global->exit_status = 1;
		return (error_return(filename));
	}
	if (ft_strncmp((*command)->words[0], "exit", len) && dup2(fd, 1) == -1)
	{
		global->exit_status = 1;
		close(fd);
		return (error_return("dup2"));
	}
	return (close(fd), 0);
}
