/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 15:46:13 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/08/09 15:29:58 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*find_cmd_file(char **cmd, char **env)
{
	char	**paths;
	int		i;
	char	*cmd_file;
	char	*temp;

	i = 0;
	paths = ft_split(get_paths(env, "PATH"), ':');
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		cmd_file = ft_strjoin(temp, cmd[0]);
		free(temp);
		if (access(cmd_file, X_OK) == 0)
		{
			free_array(paths);
			return (cmd_file);
		}
		else
			free(cmd_file);
		i++;
	}
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	free_array(paths);
	return (NULL);
}

int	get_cmd_index(t_input **command, t_pipe *exec)
{
	t_input *temp;
	int	i;

	temp = *command;
	i = 0;
	exec->cmd_i = 0;
	exec->inf_fd = -1;
	exec->outf_fd = -1;
	exec->curr = 0;
	exec->prev = 0;
	//int	here_doc;
	//int	denied_acc;
	while (temp)
	{
		if (temp->next)
			i++;
		temp = temp->next;
	}
	exec->pipe_qty = i;
	exec->pipe_fd = malloc((exec->pipe_qty) * sizeof(int *));
	return (i);
}

int	is_builtin(t_input **command)
{
	int	len;
	len = ft_strlen((*command)->words[0]);
	if(!ft_strncmp((*command)->words[0], "echo", len) || !ft_strncmp((*command)->words[0], "cd", len)
		|| !ft_strncmp((*command)->words[0], "pwd", len) || !ft_strncmp((*command)->words[0], "export", len)
		|| !ft_strncmp((*command)->words[0], "unset", len) || !ft_strncmp((*command)->words[0], "env", len)
		|| !ft_strncmp((*command)->words[0], "exit", len))
		return (1);
	return (0);
}

char	*get_paths(char **env, char *name)
{
	int		i;
	int		j;
	char	*sub;

	i = 0;
	while (env[i] != NULL)
	{
		j = 0;
		while (env[i][j] != '=')
			j++;
		sub = ft_substr(env[i], 0, j);
		if (ft_strncmp(sub, name, 4) == 0)
		{
			free(sub);
			return (env[i] + j + 1);
		}
		free(sub);
		i++;
	}
	return (NULL);
}
