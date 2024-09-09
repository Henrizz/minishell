/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:29:50 by smanriqu          #+#    #+#             */
/*   Updated: 2024/09/04 14:30:23 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expanding_var(char *str, t_global *global, int *exp_flag)
{
	size_t			expanded_len;
	t_exp_state		state;

	state.i = 0;
	state.k = 0;
	expanded_len = calc_expanded_len(str, global);
	state.expanded = (char *)malloc(expanded_len + 1);
	if (!state.expanded)
		return (NULL);
	while (str[state.i])
	{
		state.expanded = proc_expan(&state, str, global, exp_flag);
		if (!state.expanded)
		{
			free(state.expanded);
			return (NULL);
		}
	}
	state.expanded[state.k] = '\0';
	return (state.expanded);
}

char	*process_quoted_seg(char **curr, char quote, t_global *glob, int *flag)
{
	char	*start;
	char	*segment;
	char	*expanded;

	start = ++(*curr);
	while (**curr != quote && **curr != '\0')
		(*curr)++;
	segment = ft_substr(start, 0, *curr - start);
	if (quote == '"')
	{
		expanded = expanding_var(segment, glob, flag);
		free(segment);
		segment = expanded;
	}
	if (**curr == quote)
		(*curr)++;
	return (segment);
}

char	*process_non_quoted_seg(char **current, t_global *global, int *exp_flag)
{
	char	*start;
	char	*segment;
	char	*expanded;

	start = *current;
	while (**current != '\'' && **current != '"' && **current != '\0')
		(*current)++;
	segment = ft_substr(start, 0, *current - start);
	expanded = expanding_var(segment, global, exp_flag);
	free(segment);
	return (expanded);
}

char	*handle_quote(char *str, t_global *global, int *exp_flag)
{
	char	*result;
	char	*current;
	char	*segment;

	global->is_redir = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	current = str;
	while (*current != '\0')
	{
		segment = NULL;
		if (*current == '\'' || *current == '"')
			segment = process_quoted_seg(&current, *current, global, exp_flag);
		else
			segment = process_non_quoted_seg(&current, global, exp_flag);
		if (!segment)
			return (free(result), NULL);
		result = concat_and_free(result, segment);
		if (!result)
			return (NULL);
	}
	return (free(str), result);
}

void	expand_var_words(t_input *input, t_global *glob)
{
	int		i;
	t_input	*temp;

	temp = input;
	glob->home_expanded = 0;
	while (temp)
	{
		i = 0;
		while (temp->words[i])
		{
			temp->exp_word[i] = 0;
			temp->words[i] = handle_quote(temp->words[i],
					glob, &temp->exp_word[i]);
			i++;
		}
		i = 0;
		while (temp->redirections[i])
		{
			temp->exp_redir[i] = 0;
			temp->redirections[i] = handle_quote_redir(temp->redirections[i],
					glob, &temp->exp_redir[i]);
			i++;
		}
		temp = temp->next;
	}
}
