/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:49:24 by smanriqu          #+#    #+#             */
/*   Updated: 2024/09/04 15:41:53 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	syntax_error(char *str)
{
	int	i;

	i = 0;
	if (str == NULL || str[0] == '\0')
		return (1);
	if (ft_isdigit(str[0]))
		return (1);
	while (str[i] != '\0')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
		{
			if (str[i] == '=' && i > 0)
				return (0);
			else
				return (1);
		}
		i++;
	}
	return (0);
}

void	print_exported_vars(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	while (current != NULL)
	{
		if ((current->export == 1 || current->export == 2)
			&& !current->value[0])
			printf("declare -x %s\n", current->key);
		else if (current->export == 1 || current->export == 2)
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
		current = current->next;
	}
}

void	set_exported_var(t_env *env_list, char *word)
{
	char	*key;
	char	*value;
	size_t	word_len;

	key = NULL;
	value = NULL;
	word_len = ft_strlen(word);
	if (ft_strchr(word, '='))
	{
		key = ft_substr(word, 0, ft_strchr(word, '=') - word);
		value = ft_substr(word, ft_strchr(word, '=') - word + 1, word_len);
		if (!key || !value)
			return (free(key), free(value));
		set_env(key, value, env_list, 1);
	}
	else
		set_env(word, "", env_list, 2);
	free(key);
	free(value);
}

void	export(char **words, t_global *global)
{
	int	i;

	i = 1;
	if (!words[1])
	{
		print_exported_vars(global->env_list);
		global->exit_status = 0;
		return ;
	}
	while (words[i])
	{
		if (!syntax_error(words[i]))
		{
			set_exported_var(global->env_list, words[i]);
			set_env_array(global->env_list, &global->env);
			global->exit_status = 0;
		}
		else
		{
			error_identifier(words[i], "export");
			global->exit_status = 1;
		}
		i++;
	}
}
