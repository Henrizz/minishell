/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:49:53 by Henriette         #+#    #+#             */
/*   Updated: 2024/08/28 20:01:26 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* parses the command line in several steps:
	-- checks for syntax errors (TO DO)
	-- splits command line into seperate elements by whitespace but only if not inside quotes 
	(still needs adjustment for elevated case of nested quote with same quote type)
	-- distributes all separates elements into their corresponding command arrays
			- words
			- redirections
	-- launches expansion to replace environmental virables if needed */

int	parse_line(char *cmd_line, t_input **command, t_global *global)
{
	t_elements	elmts;

	if (!*cmd_line)
		return (1);
	if (!split_for_parsing(cmd_line, &elmts))
		return (1);
	if (syntax_check(&elmts) == 1)
	{
		global->exit_status = 2;
		return (1);
	}
	if (divi_up_command(command, &elmts))
	{
		free_array(elmts.array);
		global->exit_status = 1;
		return (1);
	}
	free_array(elmts.array);
	expand_var_words(*command, global->env_list, global->exit_status);
	print_arrays_testing(command);
	return (0);
}

int	divi_up_command(t_input **command, t_elements *elmts)
{
	int		i;
	t_input	*current;
	t_input	*new;

	i = 0;
	new = NULL;
	if (init_struct(command, elmts) == 1)
		return (1);
	(*command)->cmd_ind = 0;
	current = *command;
	while (elmts->array[i])
	{
		if (distribute_elements(&current, elmts, &i))
			return (1);
		if (!ft_strncmp_ed(elmts->array[i], "|", 2))
		{
			init_struct(&new, elmts);
			current->next = new;
			new->cmd_ind = current->cmd_ind + 1;
			current = new;
			i++;
		}
	}
	return (0);
}

int	distribute_elements(t_input **command, t_elements *elmts, int *i)
{
	int	k;
	int	redirect_type;

	k = 0;
	while (elmts->array[*i] && ft_strncmp_ed(elmts->array[*i], "|", 2))
	{
		redirect_type = is_redirection(elmts->array[*i]);
		if (redirect_type)
		{
			if (divi_redirect(command, elmts, i, redirect_type) == 1)
				return (1);
		}
		else
		{
			(*command)->words[k] = ft_strdup(elmts->array[*i]);
			if (!(*command)->words[k])
				return (error_return("words: ft_strdup failure"));
			k++;
		}
		(*i)++;
	}
	(*command)->words[k] = NULL;
	(*command)->redirections[(*command)->j] = NULL;
	(*command)->heredoc[(*command)->h] = NULL;
	return (0);
}

int	divi_redirect(t_input **command, t_elements *elmts, int *i, int r_type)
{
	if (r_type == IN_DETACHED)
		transfer_string(command, elmts->array[++(*i)], 0, RED_IN);
	else if (r_type == IN_ATTACHED)
		transfer_string(command, elmts->array[(*i)], 1, RED_IN);
	else if (r_type == OUT_DETACHED)
		transfer_string(command, elmts->array[++(*i)], 0, RED_OUT);
	else if (r_type == OUT_ATTACHED)
		transfer_string(command, elmts->array[(*i)], 1, RED_OUT);
	else if (r_type == APP_DETACHED)
		transfer_string(command, elmts->array[++(*i)], 0, APP_OUT);
	else if (r_type == APP_ATTACHED)
		transfer_string(command, elmts->array[(*i)], 2, APP_OUT);
	else if (r_type == HERE_DETACHED)
		(*command)->heredoc[(*command)->h++] = ft_strdup(elmts->array[++(*i)]);
	else if (r_type == HERE_ATTACHED)
		(*command)->heredoc[(*command)->h++] = ft_strdup(elmts->array[(*i)] + 2);
	if (((r_type == 1 || r_type == 2 || r_type == 3 
		|| r_type == 4) && !(*command)->redirections[(*command)->j - 1])
		|| ((r_type == 5 || r_type == 6) && !(*command)->heredoc[(*command)->h - 1]))
		return (error_return("redirections: failure to duplicate string"));
	return (0);
}

void	transfer_string(t_input **command, char *elmt, int offset, int type)
{
	(*command)->redirections[(*command)->j] = ft_strdup(elmt + offset);
	(*command)->types[(*command)->j++] = type;
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
		{
			printf("cmd_i: %d - words: %s\n", temp->cmd_ind, temp->words[i]);
			i++;
		}
		while (temp->redirections[j])
		{
			printf("cmd_i: %d - red %d: %s\n", temp->cmd_ind, temp->types[j], temp->redirections[j]);
			j++;
		}
			while (temp->heredoc[k])
		{
			printf("cmd_i: %d - heredoc: %s\n", temp->cmd_ind, temp->heredoc[k]);
			k++;
		}
		temp = temp->next;
	}
}
