/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 18:13:42 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/08/22 19:56:27 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* this function saves the original standard input and standard output in the corresponding int variable before potential redirection */
int	save_in_out(int	*stdin_copy, int *stdout_copy)
{
	*stdin_copy = dup(STDIN_FILENO); // save stdin to reset later
	*stdout_copy = dup(STDOUT_FILENO);
	if (*stdin_copy == -1 || *stdout_copy == -1)
		return (error_return("dup"));
	return (0);
}

/* this function restores the original standard in and out after the command has been executed with their potential redirection */
int	restore_in_out(int	*stdin_copy, int *stdout_copy)
{
	if (dup2(*stdin_copy, STDIN_FILENO) == -1 || dup2(*stdout_copy, STDOUT_FILENO) == -1) //reset to stdin + stdout
	{
		close(*stdin_copy);
		close(*stdout_copy);
		return (error_return("dup2"));
	}
	close(*stdin_copy);
	close(*stdout_copy);
	return (0);
}

int	make_history_file(t_global **global)
{
	char *filepath;
	char *line;

	filepath = ft_strjoin((*global)->pwd, "/.history.txt");
	if (!filepath)
		return (error_return("history file join error"));
	(*global)->history_fd = open(filepath, O_RDWR | O_CREAT | O_APPEND, 0644);
	if ((*global)->history_fd == -1)
		return (error_return("history file"));
	free(filepath);
	line = get_next_line_new((*global)->history_fd);
	while(line)
	{
		if (line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		add_history(line);
		free(line);
		line = get_next_line_new((*global)->history_fd);
	}
	return (0);
}

int	is_redirection(char *str)
{
	if (!ft_strncmp(str, "<", 2))
		return (IN_DETACHED);
	else if (!ft_strncmp(str, ">", 2))
		return (OUT_DETACHED);
	else if (!ft_strncmp(str, "<<", 3))
		return (HERE_DETACHED);
	else if (!ft_strncmp(str, ">>", 3))
		return (APP_DETACHED);
	else if (str[0] == '<' && str[1] != '<')
		return (IN_ATTACHED);
	else if (str[0] == '>' && str[1] != '>')
		return (OUT_ATTACHED);
	else if (str[0] == '<' && str[1] == '<')
		return (HERE_ATTACHED);
	else if (str[0] == '>' && str[1] == '>')
		return (APP_ATTACHED);
	else
		return (0);
}
