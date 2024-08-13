/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stephaniemanrique <stephaniemanrique@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 14:51:25 by Henriette         #+#    #+#             */
/*   Updated: 2024/08/13 16:07:02 by stephaniema      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **env)
{
	char	*cmd_line;
	t_input	*command;
	t_global *global;

	command = NULL;
	(void)argv;
	if (argc > 1)
		return (0); // for now, just to silence argc, but later maybe more actions or error handling
	global_init(&global, env);
	while(1)
	{
		cmd_line = readline("temp_prompt$ ");
		if (!cmd_line) //to check if command line pointer is NULL (in case of ctrl+D or else)
			return (exit_shell("exit\n", EXIT_SUCCESS));
		if (*cmd_line)
			add_history(cmd_line);
		if (parse_line(cmd_line, &command, (global)->env_list) != -1) //if no syntax errors have been found or line is not empty
		{
			//execute(&command, global->env_list, global->env, global->pwd);
			execute(&command, global);
			remove_heredoc(global->env, global->pwd); //to remove heredoc files after execution
			//print_array(global->env); //to check if env is being updated correctly
		}
		free(cmd_line);
		free_command(&command);
	}
	free_env_list(&global->env_list);
	free(global);
	return (0);
}
