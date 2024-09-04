/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 18:11:18 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/09/04 16:28:16 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	should_it_count_elmt(char *str, int i, t_elements *elmts, int *quoted)
{
	if ((str[i] == '"' || str[i] == '\'') && !(*quoted))
	{
		*quoted = 1;
		elmts->quote_type = str[i];
	}
	else if (str[i] == elmts->quote_type && *quoted)
		*quoted = 0;
	if (!is_whitespace(str[i]))
		elmts->is_word = 1;
	if (!(*quoted) && ((is_whitespace(str[i]) && elmts->is_word == 1) 
			|| str[i] == '|' 
			|| (is_symbol(str[i], 9) && !was_before(str, i, str[i]))))
	{
		if ((str[i] == '|' && !was_before(str, i, ' ')))
			elmts->elmt_count++;
		elmts->elmt_count++;
		elmts->is_word = 0;
	}
}

int	should_it_count_char(char c, int *inside_quote)
{
	if ((*inside_quote == 0 && !is_whitespace(c) && !is_symbol(c, PIPE))
		|| *inside_quote == 1)
		return (1);
	return (0);
}

int	is_symbol(char c, int flag)
{
	if (flag == PIPE)
	{
		if (c == '|' || c == '<' || c == '>')
			return (1);
	}
	else if (flag == REDIR)
	{
		if (c == '<' || c == '>')
			return (1);
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

void	print_arrays_testing(t_input **command)
{
	t_input	*temp;
	int		i;
	int		j;
	int		k;

	temp = *command;
	while (temp)
	{
		i = 0;
		j = 0;
		k = 0;
		while (temp->words[i])
			printf("cmd_i: %d - words: %s\n", temp->cmd_ind, temp->words[i++]);
		while (temp->redirections[j])
		{
			printf("cmd_i: %d - red %d: %s\n", temp->cmd_ind, temp->types[j],
				temp->redirections[j]);
			j++;
		}
		while (temp->heredoc[k])
			printf("cmd_i: %d - heredoc: %s\n", temp->cmd_ind,
				temp->heredoc[k++]);
		temp = temp->next;
	}
}
