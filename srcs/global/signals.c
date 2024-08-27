#include "../../includes/minishell.h"

void	signal_handler(int signum)
{
	//if inside of a heredoc then stop heredoc
	if (signum == SIGINT)
	{
		write(1,"\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	init_signals(void)
{
	rl_catch_signals = 0; //to avoid readline catching signals
	signal(SIGINT, signal_handler); //ctrl + c
	signal(SIGQUIT, SIG_IGN); //ctrl + '\'
}