/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 21:15:28 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/09/10 13:20:15 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
