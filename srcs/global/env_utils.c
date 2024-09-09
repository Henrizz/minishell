/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:32:12 by smanriqu          #+#    #+#             */
/*   Updated: 2024/09/04 14:32:23 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_env_var(t_env *env_var)
{
	if (env_var)
	{
		free(env_var->key);
		free(env_var->value);
		free(env_var);
	}
}

void	free_env_list(t_env **env_list)
{
	t_env	*current;
	t_env	*next;

	current = *env_list;
	while (current)
	{
		next = current->next;
		free_env_var(current);
		current = next;
	}
	*env_list = NULL;
}

t_env	*allocate_env_var(void)
{
	t_env	*new_var;

	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return (NULL);
	new_var->key = NULL;
	new_var->value = NULL;
	new_var->export = 0;
	new_var->next = NULL;
	return (new_var);
}

char	*get_env_value(char *var_name, t_env *env_list, int is_redir)
{
	t_env	*current;
	char	*value;
	size_t	len;

	current = env_list;
	len = ft_strlen(var_name);
	while (current)
	{
		if (ft_strncmp(var_name, current->key, len)
			== 0 && current->key[len] == '\0')
		{
			value = ft_strdup(current->value);
			return (value);
		}
		current = current->next;
	}
	if (is_redir)
	{
		value = ft_strjoin("$", var_name);
		return (value);
	}
	value = ft_strdup("");
	return (value);
}

t_env	*find_existing_env(t_env *env_list, char *key, size_t key_len)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (!ft_strncmp(current->key, key, key_len)
			&& ft_strlen(current->key) == key_len)
			return (current);
		current = current->next;
	}
	return (NULL);
}
