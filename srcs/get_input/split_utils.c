/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Henriette <Henriette@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:08:03 by Henriette         #+#    #+#             */
/*   Updated: 2024/08/20 17:09:01 by Henriette        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	advance_line(char **cmd_line, int *inside_quote, t_elements *elmts)
{
	while (is_whitespace(**cmd_line) && *inside_quote == 0)
			(*cmd_line)++;
		if ((**cmd_line == '"' || **cmd_line == '\'') && *inside_quote == 0)
		{
			*inside_quote = 1;
			elmts->quote_type = **cmd_line;
		}
		else if (**cmd_line == elmts->quote_type && *inside_quote == 1)
			*inside_quote = 0;
}

void set_quotes(char **str, int *inside_quote, t_elements *elmts)
{
	if (*inside_quote == 0 && (**str == '"' || **str == '\''))
		{
			elmts->quote_type = **str;
			*inside_quote = 1;
		}
		else if (*inside_quote == 1 && (**str == '"' || **str == '\''))
		{
			if (elmts->quote_type == '\0')
				elmts->quote_type = **str;
			else if (**str == elmts->quote_type)
				*inside_quote = 0;
		}
}

int	was_before(char *str, int i, char c)
{
	if (i == 0)
		return (1);
	if (str[i - 1] == c)
		return (1);
	return (0);
}

int	is_whitespace(char c)
{
	if (c == ' ' || c == '\n' || c == '\t')
		return (1);
	return (0);
}
