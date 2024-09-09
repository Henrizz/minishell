/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:32:27 by smanriqu          #+#    #+#             */
/*   Updated: 2024/09/04 14:32:31 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strjoin_free(char *s1, char *s2, int free_str)
{
	char	*new_str;

	new_str = ft_strjoin(s1, s2);
	if (free_str == 1)
		free(s1);
	else if (free_str == 2)
		free(s2);
	else if (free_str == 3)
	{
		free(s1);
		free(s2);
	}
	return (new_str);
}

int	populate_env_array(t_env *env_list, char **env_array)
{
	t_env	*current;
	char	*temp;
	int		i;

	current = env_list;
	i = 0;
	while (current)
	{
		env_array[i] = ft_strjoin(current->key, "=");
		if (!env_array[i])
			return (free_array(env_array), 0);
		temp = ft_strjoin(env_array[i], current->value);
		free(env_array[i]);
		if (!temp)
			return (free_array(env_array), 0);
		env_array[i++] = temp;
		current = current->next;
	}
	env_array[i] = NULL;
	return (1);
}

int	set_env_array(t_env *env_list, char ***env_array)
{
	t_env	*current;
	int		env_count;

	current = env_list;
	env_count = 0;
	while (current)
	{
		env_count++;
		current = current->next;
	}
	if (*env_array)
	{
		free_array(*env_array);
		*env_array = NULL;
	}
	*env_array = malloc(sizeof(char *) * (env_count + 1));
	if (!*env_array)
		return (0);
	if (populate_env_array(env_list, *env_array))
		return (1);
	return (0);
}

int	create_prompt(char **prompt, t_global *global)
{
	char	*value;
	char	*temp;

	value = get_env_value("USER", global->env_list);
	if (!value)
		return (1);
	temp = ft_strjoin(value, "@");
	if (temp == NULL)
	{
		free(value);
		return (1);
	}
	*prompt = ft_strjoin(temp, "minishell$ ");
	if (*prompt == NULL)
	{
		free(value);
		free(temp);
		return (1);
	}
	free(value);
	free(temp);
	return (0);
}

void	global_init(t_global **global, char **env)
{
	t_env	*env_list;
	char	**env_array;

	env_list = NULL;
	env_array = NULL;
	*global = malloc(sizeof(t_global));
	(*global)->exec = malloc(sizeof(t_pipe));
	if (!*global || !(*global)->exec)
	{
		perror("minishell: malloc");
		exit(EXIT_FAILURE);
	}
	(*global)->exit_status = 0;
	(*global)->pwd = getenv("PWD");
	if (env_init(env, &env_list))
		(*global)->env_list = env_list;
	if (set_env_array((*global)->env_list, &env_array))
		(*global)->env = env_array;
	make_history_file(global);
	(*global)->stdin_cp = -1;
	(*global)->stdout_cp = -1;
	if (create_prompt(&((*global)->prompt), *global))
		(*global)->prompt = ft_strdup("minishell$ ");
}
