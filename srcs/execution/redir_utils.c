/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 18:13:42 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/09/02 18:44:39 by hzimmerm         ###   ########.fr       */
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

void	file_error(char *file, char *mssg, t_global *glob, t_input **inpt)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(mssg, 2);
	free(file);
	free_command(inpt);
	if (!ft_strncmp(mssg, ": Permission denied\n", 21) 
		|| !ft_strncmp(mssg, ": Is a directory\n", 18))
		glob->exit_status = 126;
	else if (!ft_strncmp(mssg, ": No such file or directory\n", 29))
		glob->exit_status = 127;
	cleanup_and_exit(glob);
}
