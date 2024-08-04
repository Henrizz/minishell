/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stephaniemanrique <stephaniemanrique@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:49:53 by Henriette         #+#    #+#             */
/*   Updated: 2024/08/04 17:19:55 by stephaniema      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* parses the command line in several steps:
	-- checks for syntax errors (TO DO)
	-- splits command line into seperate elements by whitespace but only if not inside quotes (still needs adjustment for elevated case of nested quote with same quote type)
	-- distributes all separates elements into their corresponding command arrays
			- words--> done for single command, no pipe yet --> TO DO: several commands divided by pipes
			- redirections (TO DO) */

int parse_line(char *cmd_line, t_input **command, t_env *env_list)
{
	t_elements elmts;
	//t_input *temp;

	if (!*cmd_line)
		return (-1);
	//syntax error handling here
	split_for_parsing(cmd_line, &elmts);
	divi_up_command(command, &elmts);
	/*temp = *command;
	while (temp)
	{
		int	i;
		i = 0;
		while (temp->words[i])
		{
			ft_printf("%s\n", temp->words[i]);
			i++;
		}
		temp = temp->next;
	}*/
	// add redirections, heredoc, and separate commands divided by pipes
	expand_var_words(*command, env_list);
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

	k = 0;
	while (elmts->array[*i] && ft_strncmp_ed(elmts->array[*i], "|", 2))
	{
		//if (is_redirection(elmts->array[i][0]))
			//--> put from array into specific redirection array
		//else
		(*command)->words[k] = ft_strdup(elmts->array[*i]);
		if (!(*command)->words[k])
			exit_shell("failure to duplicate string", EXIT_FAILURE);
		(*i)++;
		k++;
	}
	(*command)->words[k] = NULL;
}

int	is_redirection(char c)
{
	if (c == '<' || c == '>') //c == '<<' ||  || c == '>>') these would be considered strings, so maybe this is enoough and better more details in the redirection function
		return (1);
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
	//(*command)->cmd_ind = 0; this needs to be different in every command
	(*command)->next = NULL;
	if (!(*command)->words || !(*command)->red_in || !(*command)->red_out || !(*command)->heredoc || !(*command)->app_out)
		exit_shell("memory allocation failure", EXIT_FAILURE);
	(*command)->words[0] = NULL;
	(*command)->red_in[0] = NULL;
	(*command)->red_out[0] = NULL;
	(*command)->heredoc[0] = NULL;
	(*command)->app_out[0] = NULL;
}
