/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_syntax_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:12:16 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/08/21 15:26:18 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	syntax_check(t_elements *elmts)
{
	int	i;
	

	i = 0;
	while (elmts->array[i])
	{
		if (check_for_symbols(elmts->array, i) || check_for_doubles(elmts->array, i))
		{
			free_array(elmts->array);
			return (1);
		}
		if ((!ft_strncmp(elmts->array[i], "|", 2) || !ft_strncmp(elmts->array[i], "<", 2)
			|| !ft_strncmp(elmts->array[i], ">", 2) || !ft_strncmp(elmts->array[i], "<<", 3)
			|| !ft_strncmp(elmts->array[i], ">>", 3)) && elmts->array[i + 1] == NULL)
		{
			printf("minishell: syntax error near unexpected token 'newline'\n");
			free_array(elmts->array);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_for_doubles(char **array, int i)
{
	int	value;

	value = 0;
	if (!ft_strncmp(array[i], ">>>", 3) || !ft_strncmp(array[i], "<<<", 3))
		value = printf("minishell: syntax error near unexpected token '%c'\n", array[i][0]);
	else if ((!ft_strncmp(array[i], ">>", 2) || !ft_strncmp(array[i], "<<", 2))
		&& was_before_array(array, i, REDIR))
		value = printf("minishell: syntax error near unexpected token '%.2s'\n", array[i]);
	else if ((!ft_strncmp(array[i], ">", 1) || !ft_strncmp(array[i], "<", 1))
		&& was_before_array(array, i, REDIR))
		value = printf("minishell: syntax error near unexpected token '%c'\n", array[i][0]);
	else if (!ft_strncmp(array[i], "|", 2) && was_before_array(array, i, PIPE))
		value = printf("minishell: syntax error near unexpected token '%c'\n", array[i][0]);
	return (value);
}

int	was_before_array(char **array, int i, int symbol)
{
	if (i == 0)
		return (0);
	if (symbol == REDIR)
	{
		if (!ft_strncmp(array[i - 1], "<", 2) || !ft_strncmp(array[i - 1], ">", 2) 
			|| !ft_strncmp(array[i - 1], "<<", 3) || !ft_strncmp(array[i - 1], ">>", 3))
			return (1);
	}
	if (symbol == PIPE)
	{
		if (!ft_strncmp(array[i - 1], "|", 2))
			return (1);
	}
	return (0);
}

int	check_for_symbols(char **array, int i)
{
	int	j;
	int	inside_quote;
	char quote_type;

	j = 0;
	inside_quote = 0;
	quote_type = '\0';
	while (array[i][j])
	{
		if (inside_quote == 0 && (array[i][j] == '"' || array[i][j] == '\''))
		{
			inside_quote = 1;
			quote_type = array[i][j];
		}
		else if (array[i][j] == quote_type)
		{
			inside_quote = 0;
			quote_type = '\0';
		}
		if (inside_quote == 0 && (array[i][j] == ';' || array[i][j] == '\\' 
			|| array[i][j] == '&' || array[i][j] == '(' || array[i][j] == ')' || array[i][j] == '#'))
		{
			printf("minishell: error: shell does not handle '%c'\n", array[i][j]);
			return (1);
		}
		j++;
	}
	return (0);
}
