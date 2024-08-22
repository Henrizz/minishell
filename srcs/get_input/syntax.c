/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_syntax_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:12:16 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/08/22 19:11:43 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	syntax_check(t_elements *elmts)
{
	int	i;
	int		quoted;
	char	quote_type;

	i = 0;
	while (elmts->array[i])
	{
		quoted = 0;
		quote_type = '\0';
		if (check_symbols(elmts->array[i], quoted, quote_type) || check_doubles(elmts->array, i))
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

int	check_doubles(char **array, int i)
{
	int		value;
	char	*message;

	value = 0;
	message = "minishell: syntax error near unexpected token";
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
		if (!ft_strncmp(array[i - 1], "|", 2))
			return (1);
	}
	return (0);
}

int	check_symbols(char *array, int inside_quote, int quote_type)
{
	int		j;

	j = 0;
	while (array[j])
	{
		if (inside_quote == 0 && (array[j] == '"' || array[j] == '\''))
		{
			inside_quote = 1;
			quote_type = array[j];
		}
		else if (array[j] == quote_type)
		{
			inside_quote = 0;
			quote_type = '\0';
		}
		if (inside_quote == 0 && (array[j] == ';' || array[j] == '\\' 
			|| array[j] == '&' || array[j] == '(' || array[j] == ')' || array[j] == '#'))
		{
			printf("minishell: error: shell does not handle '%c'\n", array[j]);
			return (1);
		}
		j++;
	}
	return (0);
}

int	init_struct(t_input **command, t_elements *elmts)
{
	*command = malloc(sizeof(t_input));
	if (!(*command))
		return (error_return("memory allocation failure"));
	(*command)->words = malloc((elmts->elmt_count + 1) * sizeof(char *));
	(*command)->heredoc = malloc((elmts->elmt_count + 1) * sizeof(char *));
	(*command)->redirections = malloc((elmts->elmt_count + 1) * sizeof(char *));
	(*command)->types = malloc((elmts->elmt_count + 1) * sizeof(int));
	(*command)->next = NULL;
	if (!(*command)->words || !(*command)->redirections || !(*command)->heredoc 
		|| !(*command)->types)
		return (error_return("memory allocation failure"));
	(*command)->words[0] = NULL;
	(*command)->heredoc[0] = NULL;
	(*command)->redirections[0] = NULL;
	(*command)->j = 0;
	(*command)->h = 0;
	(*command)->pid = -1;
	return (0);
}
