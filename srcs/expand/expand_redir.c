/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:29:50 by smanriqu          #+#    #+#             */
/*   Updated: 2024/09/09 19:28:36 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*handle_single_quotes(char **curr)
{
	char	*segment;
	char	*result;
	char	*quoted_segment;

	(*curr)++;
	segment = extract_segment(curr, '\'');
	if (contains_dollar_sign(segment))
	{
		result = ft_strjoin("'", segment);
		quoted_segment = ft_strjoin(result, "'");
		free(result);
		free(segment);
		segment = quoted_segment;
	}
	if (**curr == '\'')
		(*curr)++;
	return (segment);
}

char	*handle_double_quotes(char **curr, t_global *glob, int *flag)
{
	char	*segment;
	char	*expanded;

	(*curr)++;
	segment = extract_segment(curr, '"');
	if (**curr == '"')
		(*curr)++;
	expanded = expanding_var(segment, glob, flag);
	free(segment);
	return (expanded);
}

char	*quoted_seg_redir(char **curr, char quote, t_global *glob, int *flag)
{
	if (quote == '"')
		return (handle_double_quotes(curr, glob, flag));
	else if (quote == '\'')
		return (handle_single_quotes(curr));
	return (NULL);
}

char	*non_quoted_seg_redir(char **current, t_global *global, int *exp_flag)
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

char	*handle_quote_redir(char *str, t_global *global, int *exp_flag)
{
	char	*result;
	char	*current;
	char	*segment;

	global->is_redir = 1;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	current = str;
	while (*current != '\0')
	{
		segment = NULL;
		if (*current == '\'' || *current == '"')
			segment = quoted_seg_redir(&current, *current, global, exp_flag);
		else
			segment = non_quoted_seg_redir(&current, global, exp_flag);
		if (!segment)
			return (free(result), NULL);
		result = concat_and_free(result, segment);
		if (!result)
			return (NULL);
	}
	return (free(str), result);
}
