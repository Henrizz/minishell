/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Henriette <Henriette@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 14:51:25 by Henriette         #+#    #+#             */
/*   Updated: 2024/07/23 16:07:50 by Henriette        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **env)
{
	char	*cmd_line;
	t_input	*command;
	
	command = NULL;
	(void)argv;
	(void)env;
	if (argc > 1)
		return (0); // for now, later maybe more actions or error handling
	while(1)
	{
		cmd_line = readline("temp_prompt$");
		if (!cmd_line)
			return (exit_shell("exit\n", EXIT_SUCCESS));
		if (*cmd_line)
			add_history(cmd_line);
		// function to parse command line and initialise and populate input struct 
				// --> (from there also do syntax error checks and launch expansion function)
		if (parse_line(cmd_line, command) != -1) //if no syntax errors have been found or line is not empty
		{
			// function for execution (from there also launch builtin execution or seperate function)
		}
		free(cmd_line);
	}
	return (0);
}
