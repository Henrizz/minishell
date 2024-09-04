/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:32:50 by smanriqu          #+#    #+#             */
/*   Updated: 2024/09/04 14:32:52 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	display_new_line_sigint(int signum)
{
	if (signum == SIGINT)
	{
		g_global_signum = SIGINT;
		write(1, "\n", STDERR_FILENO);
		rl_on_new_line();
	}
}

void	sig_interactive_heredoc(void)
{
	signal(SIGINT, signal_handler_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_basic(void)
{
	signal(SIGINT, reset_line);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_execution(void)
{
	signal(SIGINT, display_new_line_sigint);
	signal(SIGQUIT, display_new_line_sigquit);
}

void	set_signum_and_exit_status(t_global *global)
{
	if (g_global_signum == SIGINT)
		global->exit_status = (EKEYREVOKED + SIGINT);
	else if (g_global_signum == SIGQUIT)
		global->exit_status = (EKEYREVOKED + SIGQUIT);
	g_global_signum = 0;
}
