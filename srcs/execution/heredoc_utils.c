/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 18:46:14 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/09/09 17:25:59 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*make_heredoc_filename(t_input **command, int i, t_global *global)
{
	char	*filepath;
	char	*directory;
	char	*num;

	num = make_num(i, (*command)->cmd_ind);
	if (!num)
		return (NULL);
	if (make_heredoc_directory(global) == 1)
		return (NULL);
	directory = ft_strjoin(global->pwd, "/.heredocs/.here_");
	filepath = ft_strjoin(directory, num);
	if (!filepath)
	{
		perror("minishell: ");
		global->exit_status = 1;
		return (free(num), NULL);
	}
	i++;
	free(num);
	free(directory);
	return (filepath);
}

char	*make_num(int i, int cmd_ind)
{
	char	*num1;
	char	*num2;
	char	*num;

	num1 = ft_itoa(i);
	if (!num1)
		return (NULL);
	num2 = ft_itoa(cmd_ind);
	if (!num2)
		return (NULL);
	num = ft_strjoin(num1, num2);
	if (!num)
	{
		free(num1);
		free(num2);
		return (NULL);
	}
	free(num1);
	free(num2);
	return (num);
}

int	make_heredoc_directory(t_global *global)
{
	int		pid;
	char	*cmd_file;
	char	*cmd[3];
	int		status;

	cmd[0] = "mkdir";
	cmd[1] = ft_strjoin(global->pwd, "/.heredocs");
	if (!cmd[1])
		return (error_return("error allocating heredoc path"));
	cmd[2] = NULL;
	if (access(cmd[1], R_OK | W_OK | X_OK) == 0)
		return (free(cmd[1]), 0);
	pid = fork();
	if (pid == -1)
		return (error_return("fork error"));
	if (pid == 0)
	{
		cmd_file = find_cmd_file(cmd, global->env);
		execve(cmd_file, cmd, global->env);
		exit(error_return("execve fail\n"));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		global->exit_status = WEXITSTATUS(status);
	return (free(cmd[1]), 0);
}

int	here_expand(t_heredoc *here, char *name)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	here->quoted = 0;
	here->count = 1;
	here->quote_type = '\0';
	here->flag = 0;
	here->exp = malloc((ft_strlen(name) + 1) * sizeof(char));
	if (!here->exp)
		return (1);
	while (name[i])
	{
		transfer_char(name, here, &j, &i);
		i++;
	}
	here->exp[j] = '\0';
	return (0);
}

void	transfer_char(char *name, t_heredoc *here, int *j, int *i)
{
	if ((name[*i] == '"' || name[*i] == '\'') && here->quoted == 0)
	{
		here->quoted = 1;
		here->flag = 1;
		here->quote_type = name[(*i)++];
	}
	if (here->quoted == 1 || (here->quoted == 0 
			&& (name[*i] != '"' || name[*i] != '\'')))
	{
		if ((name[*i] == '"' || name[*i] == '\'') 
			&& here->quote_type == name[*i])
		{
			here->quoted = 0;
			here->quote_type = '\0';
		}
		else
			here->exp[(*j)++] = name[*i];
	}
}
