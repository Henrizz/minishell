/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stephaniemanrique <stephaniemanrique@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 18:13:42 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/08/16 17:15:55 by stephaniema      ###   ########.fr       */
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
	//int	flag;

	//rl_clear_history();
	//flag = 0;
	filepath = ft_strjoin((*global)->pwd, "/.history.txt");
	if (!filepath)
		return (error_return("history file join error"));
	// if (access(filepath, F_OK) == -1)
	// 	flag = 1;
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
