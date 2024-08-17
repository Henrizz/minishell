/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stephaniemanrique <stephaniemanrique@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 15:45:39 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/08/17 11:51:45 by stephaniema      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_input_heredoc(t_input **command, char **env, char *pwd, int exit_status)
{
	int	i;
	int	fd;
	char	*filepath;
	char *line;

	i = 0;
	if (!(*command)->heredoc[0])
		return (0);
	if (make_heredoc_directory(env, pwd, exit_status) == 1)
		return (1);
	while ((*command)->heredoc[i])
	{
		filepath = make_heredoc_filename(command, i, pwd);
		fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			return (error_return("error making here_doc file"));
		free(filepath);
		while (1)
		{
			line = readline(">");
			if (line == NULL || !ft_strncmp(line, (*command)->heredoc[i], ft_strlen((*command)->heredoc[i])))
			{
				free(line);
				break ;
			}
			ft_putstr_fd(line, fd);
			ft_putstr_fd("\n", fd);
			free(line);
		}
		i++;
	}
	return (0);
}

char *make_heredoc_filename(t_input **command, int i, char *pwd)
{
	char *filepath;
	char *directory;

	directory = ft_strjoin(pwd, "/.heredocs/.");
	filepath = ft_strjoin(directory, (*command)->heredoc[i]);
	if (!filepath)
	{
		perror("minishell: ");
		(*command)->exit_status = 1;
		return (NULL);
	}
	i++;
	free(directory);
	return (filepath);
}

int	make_heredoc_directory(char **env, char *pwd, int exit_status)
{
	int	pid;
	char	*cmd_file;
	char	*cmd[3];
	//int	status;

	cmd_file = NULL;
	cmd[0] = "mkdir";
	cmd[1] = ft_strjoin(pwd, "/.heredocs");
	if (!cmd[1])
		return (error_return("error allocating heredoc path"));
	cmd[2] = NULL;
	pid = fork();
	if (pid == -1)
		return (error_return("fork error"));
	if (pid == 0)
	{
		cmd_file = find_cmd_file(cmd, env);
		execve(cmd_file, cmd, env);
		ft_putstr_fd("execve fail\n", 2);
		exit(EXIT_FAILURE);
	}
	else
		waitpid(pid, &exit_status, 0);
	free(cmd[1]);
	return (0);
}

int remove_heredoc(char **env, char *pwd, int exit_status)
{
	int	pid;
	char *cmd[4];
	char	*cmd_file;
	//int	status;

	cmd_file = NULL;
	cmd[0] = "rm";
	cmd[1] = "-rf";
	cmd[2] = ft_strjoin(pwd, "/.heredocs");
	if (!cmd[2])
		return (error_return("error allocating heredoc path"));
	cmd[3] = NULL;
	if (access(cmd[2], F_OK) == 0)
	{
		pid = fork();
		if (pid == -1)
		{
			free(cmd[2]);
			return (error_return("fork error"));
		}
		if (pid == 0)
		{

			cmd_file = find_cmd_file(cmd, env);
			execve(cmd_file, cmd, env);
			ft_putstr_fd("execve fail\n", 2);
			exit(EXIT_FAILURE);
		}
		else
			waitpid(pid, &exit_status, 0);
	}
	free(cmd[2]);
	return (0);
}
