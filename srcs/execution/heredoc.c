/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 15:45:39 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/08/22 15:37:28 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_input_heredoc(t_input **command, t_global *global)
{
	int	i;
	t_heredoc here;

	i = 0;
	if (!(*command)->heredoc[0])
		return (0);
	if (make_heredoc_directory(global) == 1)
		return (1);
	while ((*command)->heredoc[i])
	{
		here.filepath = make_heredoc_filename(command, i, global);
		here.fd = open(here.filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (here.fd == -1)
			return (error_return("error making here_doc file"));
		free(here.filepath);
		here_expand(&here, (*command)->heredoc[i]);
		while (1)
		{
			here.line = readline(">");
			if (here.line == NULL)
			{
				printf("minishell: warning: here-document delimited at line %d by end-of-file (wanted `%s')\n", here.count, here.expand);
				free(here.line);
				break ;
			}	
			else if (!ft_strncmp(here.line, here.expand, ft_strlen((*command)->heredoc[i])))
			{
				free(here.line);
				break ;
			}
			if (here.flag == 0)
				here.temp = expanding_var(here.line, global->env_list, global->exit_status);
			else
				here.temp = ft_strdup(here.line);
			if (!here.temp)
			{
				free(here.line);
				return (1);
			} 
			ft_putstr_fd(here.temp, here.fd);
			ft_putstr_fd("\n", here.fd);
			free(here.line);
			free(here.temp);
			here.count++;
		}
		i++;
		free(here.expand);
	}
	return (0);
}

char *make_heredoc_filename(t_input **command, int i, t_global *global)
{
	char *filepath;
	char *directory;

	directory = ft_strjoin(global->pwd, "/.heredocs/.");
	filepath = ft_strjoin(directory, (*command)->heredoc[i]);
	if (!filepath)
	{
		perror("minishell: ");
		global->exit_status = 1;
		return (NULL);
	}
	i++;
	free(directory);
	return (filepath);
}

int	make_heredoc_directory(t_global *global)
{
	int	pid;
	char	*cmd_file;
	char	*cmd[3];
	int	status;

	cmd_file = NULL;
	cmd[0] = "mkdir";
	cmd[1] = ft_strjoin(global->pwd, "/.heredocs");
	if (!cmd[1])
		return (error_return("error allocating heredoc path"));
	cmd[2] = NULL;
	pid = fork();
	if (pid == -1)
		return (error_return("fork error"));
	if (pid == 0)
	{
		cmd_file = find_cmd_file(cmd, global->env);
		execve(cmd_file, cmd, global->env);
		ft_putstr_fd("execve fail\n", 2);
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			global->exit_status = WEXITSTATUS(status);
	}
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

int here_expand(t_heredoc *here, char *name)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	here->quoted = 0;
	here->count = 1;
	here->quote_type = '\0';
	here->flag = 0;
	here->expand = malloc((ft_strlen(name) + 1) * sizeof(char));
	if (!here->expand)
		return (1);
	if (name[0] == '"' || name[0] == '\'')
		here->flag = 1;
	while (name[i])
	{
		if ((name[i] == '"' || name[i] == '\'') && here->quoted == 0)
		{	
			here->quoted = 1;
			here->quote_type = name[i++];
		}
		if (here->quoted == 1 || (here->quoted == 0 && (name[i] != '"' || name[i] != '\'')))
		{
			if ((name[i] == '"' || name[i] == '\'') && here->quote_type == name[i])
			{
				here->quoted = 0;
				here->quote_type = '\0';
			}
			else
				here->expand[j++] = name[i];
		}
		i++;
	}
	here->expand[j] = '\0';
	return (0);
}
