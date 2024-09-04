/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_len.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:30:47 by smanriqu          #+#    #+#             */
/*   Updated: 2024/09/04 14:30:48 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static size_t	handle_tilde(int *i, t_global *global)
{
	char	*value;
	size_t	len;

	value = get_env_value("HOME", global->env_list);
	if (!value)
		return (0);
	len = ft_strlen(value);
	free(value);
	(*i)++;
	return (len);
}

static size_t	handle_exit_status(int *i, t_global *global)
{
	char	*itoa_str;
	size_t	len;

	itoa_str = ft_itoa(global->exit_status);
	if (!itoa_str)
		return (0);
	len = ft_strlen(itoa_str);
	free(itoa_str);
	(*i) += 2;
	return (len);
}

static size_t	handle_env_var(int *i, char *str, t_global *global)
{
	char	*var_name;
	char	*value;
	size_t	len;

	(*i)++;
	var_name = extract_var_name(str, *i);
	if (!var_name)
		return (0);
	value = get_env_value(var_name, global->env_list);
	if (!value)
		return (0);
	len = ft_strlen(value);
	*i += ft_strlen(var_name);
	free(var_name);
	free(value);
	return (len);
}

size_t	calc_expanded_len(char *str, t_global *global)
{
	size_t	expanded_len;
	int		i;

	expanded_len = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '~')
			expanded_len += handle_tilde(&i, global);
		else if (str[i] == '$' && str[i + 1] == '?')
			expanded_len += handle_exit_status(&i, global);
		else if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
			expanded_len += handle_env_var(&i, str, global);
		else
		{
			expanded_len++;
			i++;
		}
	}
	return (expanded_len);
}

char	*extract_var_name(const char *str, int i)
{
	int		j;
	char	*var_name;

	if (str == NULL || i < 0 || str[i] == '\0')
		return (NULL);
	j = i;
	while (ft_isalnum(str[i]) || str[i] == '_')
		i++;
	var_name = ft_substr(str, j, i - j);
	if (var_name == NULL)
		return (free(var_name), NULL);
	return (var_name);
}
