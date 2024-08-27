#include "../../includes/minishell.h"

extern int global_signum;

void	reset_line(int signum)
{
	global_signum = signum;
	(void)signum;
	rl_replace_line("", 0);
	rl_on_new_line();
	write(1, "\n", STDERR_FILENO);
	rl_redisplay();
}

void	display_new_line(int signum)
{
	if (signum == SIGQUIT)
		ft_printf("Quit (core dumped)");
	write(1, "\n", STDERR_FILENO);
	rl_on_new_line();
}

void display_new_line_heredoc(int signum)
{
	global_signum = signum;
    if (signum == SIGINT || signum == SIGQUIT)
    {
        rl_replace_line("", 0); // Clears the current line in the buffer
        rl_done = 1;            // Stops the readline loop
        write(1, "\n", 1);      // Print a newline
    }
}

void	sig_non_interactive_heredoc(void)
{
	rl_catch_signals = 0;
	signal(SIGINT, display_new_line_heredoc);
	signal(SIGQUIT, display_new_line_heredoc);
}

void	sig_interactive(void)
{
	rl_catch_signals = 0;
	signal(SIGINT, reset_line);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_non_interactive(void)
{
	rl_catch_signals = 0;
	signal(SIGINT, display_new_line);
	signal(SIGQUIT, display_new_line);
}

// void	signal_handler(int signum)
// {
// 	//if inside of a heredoc then stop heredoc
// 	global_signum = signum;
// 	if (signum == SIGINT)
// 	{
// 		write(1,"\n", 1);
// 		rl_replace_line("", 0);
// 		rl_on_new_line();
// 		rl_redisplay();
// 	}
// }

// void	init_signals(void)
// {
// 	rl_catch_signals = 0; //to avoid readline catching signals
// 	signal(SIGINT, signal_handler); //ctrl + c
// 	signal(SIGQUIT, SIG_IGN); //ctrl + '\'
// }