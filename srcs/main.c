/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Henriette <Henriette@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 14:51:25 by Henriette         #+#    #+#             */
/*   Updated: 2024/08/11 14:05:43 by Henriette        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **env)
{
	char	*cmd_line;
	t_input	*command;
	t_env *env_list;
	char *pwd; //placeholder for now, to save the initial project folder to make heredoc directory inside and be able to locate later 
			//--> maybe we can add it to env_list structure ?
	command = NULL;
	env_list = NULL;
	pwd = getenv("PWD"); //same here, just placeholder, maybe this can happen where env_list is made ?
	(void)argv;
	if (argc > 1)
		return (0); // for now, just to silence argc, but later maybe more actions or error handling
	env_init(env, &env_list); // it can be part of a global initialization function
	while(1)
	{
		cmd_line = readline("temp_prompt$ ");
		if (!cmd_line) //to check if command line pointer is NULL (in case of ctrl+D or else)
		{
			remove_heredoc(env, pwd);
			return (exit_shell("exit\n", EXIT_SUCCESS));
		}
		if (*cmd_line)
			add_history(cmd_line);
		if (parse_line(cmd_line, &command, env_list) != -1) //if no syntax errors have been found or line is not empty
		{
			/*Testing builtin functions*/
			//what_builtin(command->words, env_list); // this just commented out for now if you still need it to test
			execute(&command, env_list, env, pwd);
			remove_heredoc(env, pwd);
		}
		free(cmd_line);
		free_command(&command);
	}
	//free environment list? --> check still reachables with valgrind
	return (0);
}
