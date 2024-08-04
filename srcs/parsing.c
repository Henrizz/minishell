/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Henriette <Henriette@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:49:53 by Henriette         #+#    #+#             */
/*   Updated: 2024/08/04 14:49:44 by Henriette        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* parses the command line in several steps: 
	-- checks for syntax errors (TO DO)
	-- splits command line into seperate elements by whitespace but only if not inside quotes (still needs adjustment for elevated case of nested quote with same quote type)
	-- distributes all separates elements into their corresponding command arrays 
			- words--> done for single command, no pipe yet --> TO DO: several commands divided by pipes
			- redirections (TO DO) */

int parse_line(char *cmd_line, t_input **command)
{	
	t_elements elmts;
	t_input *temp;
	
	if (!*cmd_line)
		return (-1);
	//syntax error handling here
	split_for_parsing(cmd_line, &elmts);
	divi_up_command(command, &elmts);
	temp = *command;
	while (temp)
	{
		int	i = 0;
		int j = 0;
		while (temp->words[i])
		{
			ft_printf("cmd_i: %d - words: %s\n", temp->cmd_ind, temp->words[i]);
			i++;
		}
		while (temp->red_in[j])
		{
			ft_printf("cmd_i: %d - red in: %s\n", temp->cmd_ind, temp->red_in[j]);
			j++;
		}
		temp = temp->next;
	}
	// add redirections, heredoc, and separate commands divided by pipes
	return (0);
}

void divi_up_command(t_input **command, t_elements *elmts)
{
	int	i;
	t_input *current;
	t_input *new;

	i = 0;
	new = NULL;
	//current = NULL;
	init_struct(command, elmts);
	(*command)->cmd_ind = 0;
	current = *command;
	while (elmts->array[i])
	{
		distribute_elements(&current, elmts, &i);
		if (!ft_strncmp_ed(elmts->array[i], "|", 2))
		{
			init_struct(&new, elmts);
			current->next = new;
			new->cmd_ind = current->cmd_ind + 1;
			current = new;
			i++;
		}
	}
	free_array(elmts->array);
}

void	distribute_elements(t_input **command, t_elements *elmts, int *i)
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
		(*command)->words[k] = ft_strdup(elmts->array[*i]);
		if (!(*command)->words[k])
			exit_shell("failure to duplicate string", EXIT_FAILURE);
		(*i)++;
		k++;
		(*command)->red_in[(*command)->j] = NULL;
		(*command)->red_out[(*command)->o] = NULL;
		(*command)->heredoc[(*command)->h] = NULL;
		(*command)->app_out[(*command)->p] = NULL;
	}
	(*command)->words[k] = NULL;
}

void	distribute_redirections(t_input **command, t_elements *elmts, int *i, int redirect_type)
{
	if (redirect_type == IN_DETACHED)
		(*command)->red_in[(*command)->j++] = ft_strdup(elmts->array[++(*i)]);
	else if (redirect_type == IN_ATTACHED)
		(*command)->red_in[(*command)->j++] = ft_strdup(elmts->array[(*i) + 1]);
	else if (redirect_type == OUT_DETACHED)
		(*command)->red_out[(*command)->o++] = ft_strdup(elmts->array[++(*i)]);
	else if (redirect_type == OUT_ATTACHED)
		(*command)->red_out[(*command)->o++] = ft_strdup(elmts->array[(*i) + 1]);
	else if (redirect_type == HERE_DETACHED)
		(*command)->heredoc[(*command)->h++] = ft_strdup(elmts->array[++(*i)]);
	else if (redirect_type == HERE_ATTACHED)
		(*command)->heredoc[(*command)->h++] = ft_strdup(elmts->array[(*i) + 2]);
	else if (redirect_type == APP_DETACHED)
		(*command)->app_out[(*command)->p++] = ft_strdup(elmts->array[++(*i)]);
	else if (redirect_type == APP_ATTACHED)
		(*command)->app_out[(*command)->p++] = ft_strdup(elmts->array[(*i) + 2]);
	if (((redirect_type == 1 || redirect_type == 2) && !(*command)->red_in[(*command)->j - 1])
		|| ((redirect_type == 3 || redirect_type == 4) && !(*command)->red_out[(*command)->o - 1])
		|| ((redirect_type == 5 || redirect_type == 6) && !(*command)->heredoc[(*command)->h - 1])
		|| ((redirect_type == 7 || redirect_type == 8) && !(*command)->app_out[(*command)->p - 1]))
		exit_shell("failure to duplicate string", EXIT_FAILURE);
	(*i)++;
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
	else if (str[0] == '<')
		return (IN_ATTACHED);
	else if (str[0] == '>')
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
	(*command)->red_in = (char **)malloc((elmts->elmt_count + 1) * sizeof(char *));
	(*command)->red_out = (char **)malloc((elmts->elmt_count + 1) * sizeof(char *));
	(*command)->heredoc = (char **)malloc((elmts->elmt_count + 1) * sizeof(char *));
	(*command)->app_out = (char **)malloc((elmts->elmt_count + 1) * sizeof(char *));
	(*command)->next = NULL;
	if (!(*command)->words || !(*command)->red_in || !(*command)->red_out || !(*command)->heredoc || !(*command)->app_out)
		exit_shell("memory allocation failure", EXIT_FAILURE);
	(*command)->words[0] = NULL;
	(*command)->red_in[0] = NULL;
	(*command)->red_out[0] = NULL;
	(*command)->heredoc[0] = NULL;
	(*command)->app_out[0] = NULL;
	(*command)->j = 0;
	(*command)->o = 0;
	(*command)->h = 0;
	(*command)->p = 0;
}



/*void	distribute_redirections(t_input **command, t_elements *elmts, int *i, int redirect_type)
{
	if (redirect_type == IN_DETACHED)
		(*command)->red_in[(*command)->j] = ft_strdup(elmts->array[++(*i)]);
	else if (redirect_type == IN_ATTACHED)
		(*command)->red_in[(*command)->j] = ft_strdup(elmts->array[(*i) + 1]);
	if (!(*command)->red_in[(*command)->j++])
		exit_shell("failure to duplicate string", EXIT_FAILURE);
	if (redirect_type == OUT_DETACHED)
		(*command)->red_out[(*command)->o] = ft_strdup(elmts->array[++(*i)]);
	else if (redirect_type == OUT_ATTACHED)
		(*command)->red_out[(*command)->o] = ft_strdup(elmts->array[(*i) + 1]);
	if (!(*command)->red_out[(*command)->o++])
		exit_shell("failure to duplicate string", EXIT_FAILURE);
	if (redirect_type == HERE_DETACHED)
		(*command)->heredoc[(*command)->h] = ft_strdup(elmts->array[++(*i)]);
	else if (redirect_type == HERE_ATTACHED)
		(*command)->heredoc[(*command)->h] = ft_strdup(elmts->array[(*i) + 2]);
	if (!(*command)->heredoc[(*command)->h++])
		exit_shell("failure to duplicate string", EXIT_FAILURE);
	if (redirect_type == APP_DETACHED)
		(*command)->app_out[(*command)->p] = ft_strdup(elmts->array[++(*i)]);
	else if (redirect_type == APP_ATTACHED)
		(*command)->app_out[(*command)->p] = ft_strdup(elmts->array[(*i) + 2]);
	if (!(*command)->app_out[(*command)->p++])
		exit_shell("failure to duplicate string", EXIT_FAILURE);
	(*i)++;
}*/
