/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:49:53 by Henriette         #+#    #+#             */
/*   Updated: 2024/08/22 14:47:04 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* parses the command line in several steps:
	-- checks for syntax errors (TO DO)
	-- splits command line into seperate elements by whitespace but only if not inside quotes (still needs adjustment for elevated case of nested quote with same quote type)
	-- distributes all separates elements into their corresponding command arrays
			- words
			- redirections
	-- launches expansion to replace environmental virables if needed */

int parse_line(char *cmd_line, t_input **command, t_global *global)
{
	t_elements elmts;

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
		return (1);
	}
	free_array(elmts.array);
	expand_var_words(*command, global->env_list, global->exit_status);
	//print_arrays_testing(command);
	return (0);
}

int divi_up_command(t_input **command, t_elements *elmts)
{
	int	i;
	t_input *current;
	t_input *new;

	i = 0;
	new = NULL;
	init_struct(command, elmts);
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
	int redirect_type;

	k = 0;
	while (elmts->array[*i] && ft_strncmp_ed(elmts->array[*i], "|", 2))
	{
		redirect_type = is_redirection(elmts->array[*i]);
		if (redirect_type)
			distribute_redirections(command, elmts, i, redirect_type);
		else
		{
			(*command)->words[k] = ft_strdup(elmts->array[*i]);
			if (!(*command)->words[k])
				return (error_return("words: failure to duplicate string"));
			k++;
		}
		(*i)++;
	}
	(*command)->words[k] = NULL;
	(*command)->redirections[(*command)->j] = NULL;
	(*command)->heredoc[(*command)->h] = NULL;
	return (0);
}

void	distribute_redirections(t_input **command, t_elements *elmts, int *i, int redirect_type)
{

	if (redirect_type == IN_DETACHED || redirect_type == IN_ATTACHED)
	{
		if (redirect_type == IN_DETACHED)
			(*command)->redirections[(*command)->j] = ft_strdup(elmts->array[++(*i)]);
		else if (redirect_type == IN_ATTACHED)
			(*command)->redirections[(*command)->j] = ft_strdup(elmts->array[(*i)] + 1);
		(*command)->types[(*command)->j++] = RED_IN;
	}
	else if (redirect_type == OUT_DETACHED || redirect_type == OUT_ATTACHED)
	{
		if (redirect_type == OUT_DETACHED)
			(*command)->redirections[(*command)->j] = ft_strdup(elmts->array[++(*i)]);
		else if (redirect_type == OUT_ATTACHED)
			(*command)->redirections[(*command)->j] = ft_strdup(elmts->array[(*i)] + 1);
		(*command)->types[(*command)->j++] = RED_OUT;
	}
	else if (redirect_type == HERE_DETACHED)
		(*command)->heredoc[(*command)->h++] = ft_strdup(elmts->array[++(*i)]);
	else if (redirect_type == HERE_ATTACHED)
		(*command)->heredoc[(*command)->h++] = ft_strdup(elmts->array[(*i)] + 2);
	else if (redirect_type == APP_DETACHED || redirect_type == APP_ATTACHED)
	{
		if (redirect_type == APP_DETACHED)
			(*command)->redirections[(*command)->j] = ft_strdup(elmts->array[++(*i)]);
		else if (redirect_type == APP_ATTACHED)
			(*command)->redirections[(*command)->j] = ft_strdup(elmts->array[(*i)] + 2);
		(*command)->types[(*command)->j++] = APP_OUT;
	}
	if (((redirect_type == 1 || redirect_type == 2 || redirect_type == 3 
		|| redirect_type == 4) && !(*command)->redirections[(*command)->j - 1])
		|| ((redirect_type == 5 || redirect_type == 6) && !(*command)->heredoc[(*command)->h - 1]))
		exit_shell("redirections: failure to duplicate string", EXIT_FAILURE);
}

int	is_redirection(char *str)
{
	if (!ft_strncmp(str, "<", 2))
		return (IN_DETACHED);
	else if (!ft_strncmp(str, ">", 2))
		return (OUT_DETACHED);
	else if (!ft_strncmp(str, "<<", 3))
		return (HERE_DETACHED);
	else if (!ft_strncmp(str, ">>", 3))
		return (APP_DETACHED);
	else if (str[0] == '<' && str[1] != '<')
		return (IN_ATTACHED);
	else if (str[0] == '>' && str[1] != '>')
		return (OUT_ATTACHED);
	else if (str[0] == '<' && str[1] == '<')
		return (HERE_ATTACHED);
	else if (str[0] == '>' && str[1] == '>')
		return (APP_ATTACHED);
	else
		return (0);
}

void	init_struct(t_input **command, t_elements *elmts)
{
	*command = malloc(sizeof(t_input));
	if (!(*command))
		exit_shell("memory allocation failure", EXIT_FAILURE);
	(*command)->words = (char **)malloc((elmts->elmt_count + 1) * sizeof(char *));
	(*command)->heredoc = (char **)malloc((elmts->elmt_count + 1) * sizeof(char *));
	(*command)->redirections = (char **)malloc((elmts->elmt_count + 1) * sizeof(char *));
	(*command)->types = (int *)malloc((elmts->elmt_count + 1) * sizeof(int));
	(*command)->next = NULL;
	if (!(*command)->words || !(*command)->redirections || !(*command)->heredoc || !(*command)->types)
		exit_shell("memory allocation failure", EXIT_FAILURE);
	(*command)->words[0] = NULL;
	(*command)->heredoc[0] = NULL;
	(*command)->redirections[0] = NULL;
	(*command)->j = 0;
	(*command)->h = 0;
	(*command)->pid = -1;
}


void print_arrays_testing(t_input **command)
{
	t_input *temp;
	int	i;
	int j;
	int k;

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
