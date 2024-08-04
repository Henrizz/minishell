/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stephaniemanrique <stephaniemanrique@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 14:51:25 by Henriette         #+#    #+#             */
/*   Updated: 2024/08/03 12:32:59 by stephaniema      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **env)
{
	char	*cmd_line;
	t_input	*command;
	t_env *env_list;

	command = NULL;
	env_list = NULL;
	(void)argv;
	(void)env;
	if (argc > 1)
		return (0); // for now, just to silence argc, but later maybe more actions or error handling
	env_init(env, &env_list); // it can be part of a global initialization function
	while(1)
	{
		cmd_line = readline("temp_prompt$ ");
		if (!cmd_line) //to check if command line pointer is NULL (in case of ctrl+D or else)
			return (exit_shell("exit\n", EXIT_SUCCESS));
		if (*cmd_line)
			add_history(cmd_line);

		// function to parse command line and initialise and populate input struct
				// --> (from there also do syntax error checks and launch expansion function)
		if (parse_line(cmd_line, &command, env_list) != -1) //if no syntax errors have been found or line is not empty
		{
			/*Testing builtin functions*/
			what_builtin(command->words, env_list);
			// function for execution (from there also launch builtin execution or seperate function)
		}

		free(cmd_line);
		free_command(&command);
	}
	return (0);
}
