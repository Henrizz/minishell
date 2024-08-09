/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 14:51:25 by Henriette         #+#    #+#             */
/*   Updated: 2024/08/09 14:09:26 by hzimmerm         ###   ########.fr       */
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
		if (parse_line(cmd_line, &command, env_list) != -1) //if no syntax errors have been found or line is not empty
		{
			/*Testing builtin functions*/
			//what_builtin(command->words, env_list);
			execute(&command, env_list, env);
		}
		free(cmd_line);
		free_command(&command);
	}
	//free environment list? --> check still reachables with valgrind
	return (0);
}
