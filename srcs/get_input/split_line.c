/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Henriette <Henriette@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/20 09:58:49 by Henriette         #+#    #+#             */
/*   Updated: 2024/08/17 23:09:37 by Henriette        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* splits the entire command line into separate elements ("tokens"), delimited by whitesppace and taking into account when inside quotes 
	-- still missing: feature to handle nested quotes when quotes inside and outside have same quote type -- */

char **split_for_parsing(char *cmd_line, t_elements *elmts)
{
	int	i;
	int	inside_quote;
	
	i = 0;
	inside_quote = 0;
	elmts->is_word = 0;
	elmts->quote_type = '\0';
	count_elements(cmd_line, elmts);
	//printf("elements qty: %d\n", elmts->elmt_count);
	elmts->array = (char **)malloc((elmts->elmt_count + 1) * sizeof(char *));
	if (!elmts->array)
		return (NULL);
	while (*cmd_line)
	{
		/*while (is_whitespace(*cmd_line) && inside_quote == 0)
			cmd_line++;
		if ((*cmd_line == '"' || *cmd_line == '\'') && inside_quote == 0)
		{
			inside_quote = 1;
			elmts->quote_type = *cmd_line;
		}
		else if (*cmd_line == elmts->quote_type && inside_quote == 1)
			inside_quote = 0;*/
		advance_line(&cmd_line, &inside_quote, elmts);
		if (*cmd_line && (!is_whitespace(*cmd_line) || (is_whitespace(*cmd_line) && inside_quote == 1)))
			elmts->array[i++] = ft_strdup_delim(&cmd_line, &inside_quote, elmts);
		elmts->quote_type = '\0';
	}
	elmts->array[i] = NULL;
	if (inside_quote == 1)
	{
		ft_putstr_fd("minishell: error: unclosed quote\n", 2);
		free_array(elmts->array);
		return (NULL);
	}
	return (elmts->array);
}

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

char *ft_strdup_delim(char **str, int *inside_quote, t_elements *elmts)
{
	int	i;
	char	*dup;

	i = 0;
	elmts->quote_type = '\0';
	//elmts->char_count = count_characters(*str, inside_quote);
	//printf("characters qty: %d\n", elmts->char_count);
	dup = (char *)malloc((count_characters(*str, inside_quote) + 1) * sizeof(char));
	if (!dup)
		return (NULL);
	if (*inside_quote == 0 && **str == '|')
	{
		dup[i++] = **str;
		(*str)++;
		dup[i] = '\0';
		return (dup);
	}
	while (*inside_quote == 0 && (**str == '<' || **str == '>'))
	{
		dup[i++] = **str;
		(*str)++;
	}
	while (**str && ((*inside_quote == 0 && !is_whitespace(**str) && **str != '|' && **str != '<' && **str != '>') || *inside_quote == 1))
	{
		set_quotes(str, inside_quote, elmts);
		dup[i++] = **str;
		(*str)++;
	}
	dup[i] = '\0';
	//printf("dup: %s\n", dup);
	return (dup);
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

int	count_characters(char *str, int *inside_quote)
{
	int	i;
	int	count;
	char	quote_type;

	i = 0;
	count = 0;
	quote_type = '\0';
	if (str[i] == '|')
		count = 1;
	while (str[i] && ((*inside_quote == 0 && !is_whitespace(str[i]) && str[i] != '|' && str[i] != '>' && str[i] != '<') || ((str[i] == '>' || str[i] == '<') && was_before(str, i, str[i])) || *inside_quote == 1))
	{
		if (*inside_quote == 0 && (str[i] == '"' || str[i] == '\''))
		{
			quote_type = str[i];
			*inside_quote = 1;
		}
		else if (*inside_quote == 1 && (str[i] == '"' || str[i] == '\''))
		{
			if (quote_type == '\0')
				quote_type = str[i];
			else if(str[i] == quote_type)
				*inside_quote = 0;
		}
		count++;
		i++;
	}
	return (count);
}

void	count_elements(char *str, t_elements *elmts)
{
	int	i;
	int	inside_quote;
	
	elmts->is_word = 0;
	elmts->elmt_count = 0;
	i = 0;
	inside_quote = 0;
	while (str[i])
	{
		if ((str[i] == '"' || str[i] == '\'') && inside_quote == 0)
		{
			inside_quote = 1;
			elmts->quote_type = str[i];
		}
		else if (str[i] == elmts->quote_type && inside_quote == 1)
			inside_quote = 0;
		if (!is_whitespace(str[i]))
			elmts->is_word = 1;
		if ((is_whitespace(str[i]) && elmts->is_word == 1 && inside_quote == 0) || (inside_quote == 0 && (str[i] == '|' || ((str[i] == '<' || str[i] == '>') && !was_before(str, i, str[i])))))
		{
			if ((str[i] == '|' && !was_before(str, i, ' ')))
				elmts->elmt_count++;
			elmts->elmt_count++;
			elmts->is_word = 0;
		}
		//printf("letter: %c, inside quote: %d, inside word: %d\n", str[i], inside_quote, elmts->is_word);
		i++;
	}
	if (elmts->is_word == 1)
		elmts->elmt_count++;
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
