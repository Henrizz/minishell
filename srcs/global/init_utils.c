/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:32:36 by smanriqu          #+#    #+#             */
/*   Updated: 2024/09/04 14:32:39 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		printf("%s\n", array[i]);
		i++;
	}
}

void	append_new_var(t_env **env_list, t_env *new_var)
{
	t_env	*last;

	if (*env_list == NULL)
		*env_list = new_var;
	else
	{
		last = *env_list;
		while (last->next != NULL)
			last = last->next;
		last->next = new_var;
	}
}

void	reset_line(int signum)
{
	g_global_signum = signum;
	(void)signum;
	write(1, "\n", STDERR_FILENO);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	signal_handler_heredoc(int signal)
{
	if (signal == SIGINT)
	{
		g_global_signum = SIGINT;
		write(1, "\n", 1);
		rl_done = 1;
		ioctl(1, TIOCSTI, "");
	}
}

void	display_new_line_sigquit(int signum)
{
	if (signum == SIGQUIT)
	{
		g_global_signum = SIGQUIT;
		printf("Quit (core dumped)\n");
	}
}
