/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 09:58:49 by Henriette         #+#    #+#             */
/*   Updated: 2024/09/03 18:13:59 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**split_for_parsing(char *str, t_elements *elmts)
{
	int	i;
	int	quoted;

	i = 0;
	quoted = 0;
	elmts->is_word = 0;
	elmts->quote_type = '\0';
	count_elements(str, elmts);
	elmts->array = (char **)malloc((elmts->elmt_count + 1) * sizeof(char *));
	if (!elmts->array)
		return (NULL);
	while (*str)
	{
		advance_line(&str, &quoted, elmts);
		if (*str && (!is_whitespace(*str) || (is_whitespace(*str) && quoted)))
			elmts->array[i++] = ft_strdup_delim(&str, &quoted, elmts);
		elmts->quote_type = '\0';
	}
	elmts->array[i] = NULL;
	if (quoted == 1)
	{
		ft_putstr_fd("minishell: syntax error: unexpected end of file\n", 2);
		return (free_array(elmts->array), NULL);
	}
	return (elmts->array);
}

char	*ft_strdup_delim(char **str, int *quoted, t_elements *elmts)
{
	int		i;
	char	*dup;

	i = 0;
	elmts->quote_type = '\0';
	dup = (char *)malloc((count_chars(*str, quoted) + 1) * sizeof(char));
	if (!dup)
		return (NULL);
	if (*quoted == 0 && **str == '|')
	{
		dup[i++] = *(*str)++;
		dup[i] = '\0';
		return (dup);
	}
	while (*quoted == 0 && (**str == '<' || **str == '>'))
		dup[i++] = *(*str)++;
	while (**str && (*quoted 
			|| (!(*quoted) && !is_whitespace(**str) && !is_symbol(**str, 10))))
	{
		set_quotes(str, quoted, elmts);
		dup[i++] = *(*str)++;
	}
	dup[i] = '\0';
	return (dup);
}

int	count_chars(char *str, int *inside_quote)
{
	int		i;
	int		count;
	char	quote_type;

	i = 0;
	count = 0;
	quote_type = '\0';
	if (str[i] == '|')
		count = 1;
	while (str[i] && ((is_symbol(str[i], 9) && was_before(str, i, str[i]))
			|| (should_it_count_char(str[i], inside_quote))))
	{
		if ((str[i] == '"' || str[i] == '\'') && *inside_quote == 0)
		{
			quote_type = str[i];
			*inside_quote = 1;
		}
		else if ((str[i] == '"' || str[i] == '\'') 
			&& *inside_quote == 1 && str[i] == quote_type)
			*inside_quote = 0;
		count++;
		i++;
	}
	return (count);
}

void	count_elements(char *str, t_elements *elmts)
{
	int	i;
	int	quoted;

	elmts->is_word = 0;
	elmts->elmt_count = 0;
	i = 0;
	quoted = 0;
	while (str[i])
	{
		should_it_count_elmt(str, i, elmts, &quoted);
		i++;
	}
	if (elmts->is_word == 1)
		elmts->elmt_count++;
}
