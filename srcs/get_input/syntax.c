/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Henriette <Henriette@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:12:16 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/09/06 17:07:10 by Henriette        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	syntax_check(t_elements *elm)
{
	int		i;
	int		quoted;
	char	qtype;

	i = 0;
	while (elm->array[i])
	{
		quoted = 0;
		qtype = '\0';
		if (check_symbols(elm->array[i], quoted, qtype) 
			|| check_doubles(elm->array, i))
			return (free_array(elm->array), 1);
		if ((!ft_strncmp(elm->array[i], "|", 2) 
				|| !ft_strncmp(elm->array[i], "<", 2) 
				|| !ft_strncmp(elm->array[i], ">", 2) 
				|| !ft_strncmp(elm->array[i], "<<", 3)
				|| !ft_strncmp(elm->array[i], ">>", 3)) && !elm->array[i + 1])
		{
			printf("minishell: syntax error near unexpected token 'newline'\n");
			return (free_array(elm->array), 1);
		}
		i++;
	}
	return (0);
}

int	check_doubles(char **array, int i)
{
	int		value;
	char	*message;

	value = 0;
	message = "minishell: syntax error near unexpected token";
	if (!ft_strncmp(array[0], "|", 2))
		value = printf("%s '%c'\n", message, array[0][0]);
	if (!ft_strncmp(array[i], ">>>", 3) || !ft_strncmp(array[i], "<<<", 3))
		value = printf("%s '%c'\n", message, array[i][0]);
	else if ((!ft_strncmp(array[i], ">>", 2) || !ft_strncmp(array[i], "<<", 2))
		&& was_before_array(array, i, REDIR))
		value = printf("%s '%.2s'\n", message, array[i]);
	else if ((!ft_strncmp(array[i], ">", 1) || !ft_strncmp(array[i], "<", 1))
		&& was_before_array(array, i, REDIR))
		value = printf("%s '%c'\n", message, array[i][0]);
	else if (!ft_strncmp(array[i], "|", 2) && was_before_array(array, i, PIPE))
		value = printf("%s '%c'\n", message, array[i][0]);
	return (value);
}

int	was_before_array(char **array, int i, int symbol)
{
	if (i == 0)
		return (0);
	if (symbol == REDIR)
	{
		if (!ft_strncmp(array[i - 1], "<", 2) 
			|| !ft_strncmp(array[i - 1], ">", 2) 
			|| !ft_strncmp(array[i - 1], "<<", 3)
			|| !ft_strncmp(array[i - 1], ">>", 3))
			return (1);
	}
	if (symbol == PIPE)
	{
		if (!ft_strncmp(array[i - 1], "|", 2)
			|| !ft_strncmp(array[i - 1], "<", 2)
			|| !ft_strncmp(array[i - 1], ">", 2))
			return (1);
	}
	return (0);
}

int	check_symbols(char *array, int quoted, int quote_type)
{
	int		j;

	j = 0;
	while (array[j])
	{
		if (quoted == 0 && (array[j] == '"' || array[j] == '\''))
		{
			quoted = 1;
			quote_type = array[j];
		}
		else if (array[j] == quote_type)
		{
			quoted = 0;
			quote_type = '\0';
		}
		if (!quoted && (array[j] == ';' || array[j] == '\\' || array[j] == '&' 
				|| array[j] == '(' || array[j] == ')' || array[j] == '#'))
		{
			printf("minishell: error: shell does not handle '%c'\n", array[j]);
			return (1);
		}
		j++;
	}
	return (0);
}
