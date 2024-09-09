/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:29:50 by smanriqu          #+#    #+#             */
/*   Updated: 2024/09/09 18:31:11 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Helper function to extract the content inside quotes
char	*extract_segment(char **curr, char quote)
{
	char	*start;

	start = *curr;
	while (**curr != quote && **curr != '\0')
		(*curr)++;
	return (ft_substr(start, 0, *curr - start));
}

// Function to check if a string contains a '$'
int	contains_dollar_sign(const char *str)
{
	return (ft_strchr(str, '$') != NULL);
}

// Handle the content inside single quotes
char *handle_single_quotes(char **curr)
{
	char	*segment;
	char	*result;
	char	*quoted_segment;

	(*curr)++; // Move past the opening single quote
	segment = extract_segment(curr, '\''); // Extract the segment inside quotes
	if (contains_dollar_sign(segment))
	{// Preserve quotes if '$' is found anywhere in the segment
		result = ft_strjoin("'", segment);  // Add opening single quote
		quoted_segment = ft_strjoin(result, "'"); // Add closing single quote
		free(result);
		free(segment);
		segment = quoted_segment;
    }
    if (**curr == '\'')
		(*curr)++; // Move past the closing single quote
	return (segment); // Return the segment (with quotes if necessary)
}

// Handle the content inside double quotes and expand variables
char	*handle_double_quotes(char **curr, t_global *glob, int *flag)
{
	char	*segment;
	char	*expanded;

	(*curr)++; // Move past the opening double quote
	segment = extract_segment(curr, '"'); // Extract the segment inside quotes

	if (**curr == '"')
		(*curr)++; // Move past the closing double quote
	expanded = expanding_var(segment, glob, flag); // Expand variables
	free(segment);
	return (expanded);
}

// Main function to process the quoted segment based on quote type
char	*process_quoted_seg_redir(char **curr, char quote, t_global *glob, int *flag)
{
	if (quote == '"')
		return (handle_double_quotes(curr, glob, flag));
	else if (quote == '\'')
		return (handle_single_quotes(curr));
	return (NULL); // Fallback in case of error
}

/*char	*process_quoted_seg_redir(char **curr, char quote, t_global *glob, int *flag)
{
	char	*start;
	char	*segment;
	char	*expanded;

	if (quote == '"')
		start = ++(*curr);
	else if (quote == '\'')
	{
		start = *curr;
		(*curr)++;
		while (**curr != quote && **curr != '\0')
			(*curr)++;
		segment = ft_substr(start, 0, (*curr - start) + 1);

		if (**curr == quote)
			(*curr)++;
		return (segment);
	}
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
}*/

char	*process_non_quoted_seg_redir(char **current, t_global *global, int *exp_flag)
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

	result = ft_strdup("");
	if (!result)
		return (NULL);
	current = str;
	while (*current != '\0')
	{
		segment = NULL;
		if (*current == '\'' || *current == '"')
			segment = process_quoted_seg_redir(&current, *current, global, exp_flag);
		else
			segment = process_non_quoted_seg_redir(&current, global, exp_flag);
		if (!segment)
			return (free(result), NULL);

		result = concat_and_free(result, segment);
		if (!result)
			return (NULL);
	}
	return (free(str), result);
}