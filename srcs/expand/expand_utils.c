/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:30:36 by smanriqu          #+#    #+#             */
/*   Updated: 2024/09/04 14:30:38 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*handle_home(t_exp_state *state, t_global *global)
{
	char	*temp_value;
	char	*value;

	value = get_env_value("HOME", global->env_list, global->is_redir);
	if (!value)
		return (NULL);
	temp_value = value;
	while (*value)
		state->expanded[(state->k)++] = *value++;
	global->home_expanded = 1;
	free(temp_value);
	state->i++;
	return (state->expanded);
}

char	*handle_exit(t_exp_state *state, t_global *global)
{
	char	*temp_value;
	char	*value;

	value = ft_itoa(global->exit_status);
	if (!value)
		return (NULL);
	temp_value = value;
	while (*value)
		state->expanded[(state->k)++] = *value++;
	free(temp_value);
	state->i += 2;
	return (state->expanded);
}

char	*handle_var(t_exp_state *state, char *s, t_global *glob, int *flag)
{
	char	*temp_value;
	char	*var_name;
	char	*value;

	var_name = extract_var_name(s, (++state->i));
	if (!var_name)
		return (NULL);
	value = get_env_value(var_name, glob->env_list, glob->is_redir);
	if (!value)
		return (NULL);
	temp_value = value;
	while (*value)
		state->expanded[(state->k)++] = *value++;
	state->i += ft_strlen(var_name);
	free(var_name);
	free(temp_value);
	*flag = 1;
	return (state->expanded);
}

char	*proc_expan(t_exp_state *state, char *s, t_global *glob, int *flag)
{
	if (s[state->i] == '~')
		return (handle_home(state, glob));
	else if (s[state->i] == '$' && s[state->i + 1] == '?')
		return (handle_exit(state, glob));
	else if (s[state->i] == '$' && (ft_isalnum(s[state->i + 1])
			|| s[state->i + 1] == '_'))
		return (handle_var(state, s, glob, flag));
	state->expanded[(state->k)++] = s[(state->i)++];
	return (state->expanded);
}

char	*concat_and_free(char *s1, char *s2)
{
	char	*new_str;

	new_str = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!new_str)
	{
		free(s1);
		free(s2);
		return (NULL);
	}
	ft_strlcpy(new_str, s1, ft_strlen(s1) + 1);
	ft_strlcat(new_str, s2, ft_strlen(s1) + ft_strlen(s2) + 1);
	free(s1);
	free(s2);
	return (new_str);
}
