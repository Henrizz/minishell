/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 14:51:25 by Henriette         #+#    #+#             */
/*   Updated: 2024/09/03 20:27:12 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t	g_global_signum = 0;

int	main(int argc, char **argv, char **env)
{
	char		*cmd_line;
	t_input		*command;
	t_global	*global;

	command = NULL;
	(void)argv;
	if (argc > 1)
		return (printf("minishell: error: does not take arguments\n"), 1);
	global_init(&global, env);
	while (1)
	{
		sig_basic();
		cmd_line = readline(global->prompt);
		if (!cmd_line)
			shell_exit(global);
		set_signum_and_exit_status(global);
		if (*cmd_line)
			custom_add_history(cmd_line, global);
		if (parse_line(cmd_line, &command, global) != 1)
			set_signals_and_execute(&command, global);
		free(cmd_line);
		free_command(&command);
	}
	cleanup_and_exit(global);
	return (0);
}

void	custom_add_history(char *cmd_line, t_global *global)
{
	add_history(cmd_line);
	ft_putstr_fd(cmd_line, global->history_fd);
	ft_putstr_fd("\n", global->history_fd);
}

void	set_signals_and_execute(t_input **command, t_global *global)
{
	sig_interactive_heredoc();
	execute(command, global);
	remove_heredoc(global->env, global->pwd, global->exit_status);
}
