/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Henriette <Henriette@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 14:51:25 by Henriette         #+#    #+#             */
/*   Updated: 2024/07/13 15:45:21 by Henriette        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **env)
{
	char	*cmd_line;
	//t_input	*command;
	
	//command = NULL;
	(void)argv;
	(void)env;
	if (argc > 1)
		return (0); // for now, just to silence argc, but later maybe more actions or error handling
	while(1)
	{
		cmd_line = readline("temp_prompt$");
		if (!cmd_line) //to check if command line pointer is NULL (in case of ctrl+D or else)
			return (exit_shell("exit\n", EXIT_SUCCESS));
		if (*cmd_line) //to check if first char of command line is not nullterminator (so if it's not empty line)
		{
			add_history(cmd_line);
			// function to parse command line and initialise and populate input struct 
					// --> (from there also do syntax error checks and launch expansion function)
			// function for execution (from there also launch builtin execution)
		}
		free(cmd_line);
	}
	return (0);
}
