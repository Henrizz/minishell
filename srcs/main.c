/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stephaniemanrique <stephaniemanrique@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 14:51:25 by Henriette         #+#    #+#             */
/*   Updated: 2024/08/28 00:06:35 by stephaniema      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int global_signum = 0;

int	main(int argc, char **argv, char **env)
{
	char		*cmd_line;
	t_input		*command;
	t_global	*global;

	command = NULL;
	(void)argv;
	if (argc > 1)
		return (0); // for now, just to silence argc, but later maybe more actions or error handling
	global_init(&global, env);
	while (1)
	{
		sig_interactive();
		//init_signals();
		cmd_line = readline("temp_prompt$ ");
		if (!cmd_line) //to check if command line pointer is NULL (in case of ctrl+D or else)
			shell_exit(global);
		sig_non_interactive();
		if (*cmd_line)
		{
			add_history(cmd_line);
			ft_putstr_fd(cmd_line, global->history_fd);
			ft_putstr_fd("\n", global->history_fd);
		}
		if (parse_line(cmd_line, &command, global) != 1) //if no syntax errors have been found or line is not empty
		{
			execute(&command, global);
			remove_heredoc(global->env, global->pwd, global->exit_status); //to remove heredoc files after execution
		}
		free(cmd_line);
		free_command(&command);
	}
	cleanup_and_exit(global);
	// free_env_list(&global->env_list);
	// free_array(global->env);
	// free(global);
	return (0);
}
