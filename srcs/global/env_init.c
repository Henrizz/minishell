/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:32:00 by smanriqu          #+#    #+#             */
/*   Updated: 2024/09/04 14:35:54 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	update_existing_env(t_env *current, char *value, int export_flag)
{
	free(current->value);
	current->value = ft_strdup(value);
	if (!current->value)
		return ;
	current->export = export_flag;
}

void	set_env(char *key, char *value, t_env *env_list, int export_flag)
{
	size_t	key_len;
	t_env	*existing_env;
	t_env	*new_var;

	key_len = ft_strlen(key);
	existing_env = find_existing_env(env_list, key, key_len);
	if (existing_env)
	{
		update_existing_env(existing_env, value, export_flag);
		return ;
	}
	new_var = new_env_var(key, export_flag);
	if (!new_var)
		return ;
	free(new_var->value);
	new_var->value = ft_strdup(value);
	if (!new_var->value)
	{
		free_env_var(new_var);
		return ;
	}
	append_new_var(&env_list, new_var);
}

int	set_key_value(t_env *env_var, char *str)
{
	char	*equal_sign;
	size_t	len;

	equal_sign = ft_strchr(str, '=');
	len = ft_strlen(str);
	if (equal_sign)
	{
		env_var->key = ft_substr(str, 0, equal_sign - str);
		env_var->value = ft_substr(str, equal_sign - str
				+ 1, len - (equal_sign - str + 1));
	}
	else
	{
		env_var->key = ft_strdup(str);
		env_var->value = ft_strdup("");
	}
	return (env_var->key && env_var->value);
}

t_env	*new_env_var(char *str, int export)
{
	t_env	*new_var;

	new_var = allocate_env_var();
	if (!new_var)
		return (NULL);
	if (!set_key_value(new_var, str))
	{
		free_env_var(new_var);
		return (NULL);
	}
	new_var->export = export;
	return (new_var);
}

int	env_init(char **env, t_env **env_list)
{
	t_env	*new_var;

	while (*env)
	{
		new_var = new_env_var(*env, 1);
		if (!new_var)
		{
			free_env_list(env_list);
			return (0);
		}
		append_new_var(env_list, new_var);
		env++;
	}
	return (1);
}
