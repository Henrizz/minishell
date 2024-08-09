/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 15:45:39 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/08/09 16:58:14 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_input_heredoc(t_input **command, char **env)
{
	int	i;
	int	fd;
	char	*filepath;
	char *line;

	i = 0;
	if (!(*command)->heredoc[0])
		return (0);
	if (make_heredoc_directory(env) == -1)
		return (-1);
	
	while ((*command)->heredoc[i])
	{
		filepath = make_heredoc_filename(command, i);
		fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			return (error_return("minishell: error making here_doc file"));
		free(filepath);
		while (1)
		{
			line = get_next_line_new(0);
			if (line == NULL || !ft_strncmp(line, (*command)->heredoc[i], ft_strlen((*command)->heredoc[i])))
			{
				free(line);
				break ;
			}
			ft_putstr_fd(line, fd);
			free(line);
		}
		i++;
	}
	return (0);
}

char *make_heredoc_filename(t_input **command, int i)
{
	char *filepath;


	filepath = ft_strjoin("/home/hzimmerm/projects/minishell/.heredocs/.", (*command)->heredoc[i]);
	if (!filepath)
	{
		ft_putstr_fd("minishell: error making heredoc filepath", 2);
		return (NULL);
	}
	//printf("%s\n", filepath);
	i++;
	return (filepath);
}

int	make_heredoc_directory(char **env)
{
	int	pid;
	char	*cmd_file;
	char *cmd[3] = {"mkdir", ".heredocs", NULL};

	cmd_file = NULL;
	pid = fork();
	if (pid == -1)
		return (error_return("minishell: fork error"));
	if (pid == 0)
	{
		cmd_file = find_cmd_file(cmd, env);
		if (cmd_file == NULL)
		{
			free(cmd_file);
			exit(EXIT_FAILURE);
		}
		execve(cmd_file, cmd, env);
		//printf("execve fail\n"); make better error catch
		return (-1);
	}
	else 
		waitpid(pid, NULL, 0);
	return (0);
}


/*int	permissions_and_open(int argc, char **argv, t_multi *pipex)
{
	int		fd;
	char	*infile;
	char	*outfile;

	infile = argv[1];
	outfile = argv[argc - 1];
	pipex->cmd_qty = argc - 3 - pipex->here_doc;
	create_pipes(pipex);
	if (!ft_strncmp(infile, "here_doc", 8))
	{
		fd = open(".here_doc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			return (error_exit("here_doc"));
		get_input_append(fd, argv, outfile, pipex);
	}
	else
		open_files(infile, outfile, pipex, O_TRUNC);
	return (1);
}

void	get_input_append(int fd, char **argv, char *outfile, t_multi *pipex)
{
	char	*line;

	while (1)
	{
		line = get_next_line_new(0);
		if (line == NULL || !ft_strncmp(line, argv[2], ft_strlen(argv[2])))
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd);
		free(line);
	}
	open_files(".here_doc", outfile, pipex, O_APPEND);
	remove_delimiter(argv);
}*/
