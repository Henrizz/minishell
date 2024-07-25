/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Henriette <Henriette@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:49:53 by Henriette         #+#    #+#             */
/*   Updated: 2024/07/24 17:21:46 by Henriette        ###   ########.fr       */
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
	int	i;
	
	i = 0;
	if (!*cmd_line)
		return (-1);
	//syntax error handling here
	split_for_parsing(cmd_line, &elmts);
	divi_up_command(command, &elmts);
	/*while ((*command)->words[i])
	{
		ft_printf("%s\n", (*command)->words[i]);
		i++;
	}*/
	// add redirections, heredoc, and separate commands divided by pipes
	return (0);
}

void divi_up_command(t_input **command, t_elements *elmts)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	init_struct(command, elmts);
	(*command)->cmd_ind = 0;
	while (elmts->array[i])
	{
		//if (is_redirection(elmts->array[i][0]))
			//--> put from array into specific redirection array
		//else
		(*command)->words[k] = ft_strdup(elmts->array[i]);
		if (!(*command)->words[k])
			exit_shell("failure to duplicate string", EXIT_FAILURE);
		i++;
		k++;
	}
	(*command)->words[k] = NULL;
	free_array(elmts->array);
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
