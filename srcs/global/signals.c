#include "../../includes/minishell.h"

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
		//write(1, "\n", STDERR_FILENO);
		//rl_on_new_line();
	}
}

void	display_new_line_sigint(int signum)
{
	if (signum == SIGINT)
	{
		g_global_signum = SIGINT;
		//printf("\n");
		write(1, "\n", STDERR_FILENO);
		rl_on_new_line();
	}
}

/*void display_new_line_heredoc(int signum)
{
	global_signum = signum;
	ft_printf("signal in handler= %d\n", signum);
    if (signum == SIGINT || signum == SIGQUIT)
    {
        rl_replace_line("", 0); // Clears the current line in the buffer
        rl_done = 1;            // Stops the readline loop
        write(1, "\n", 1);      // Print a newline
    }
}*/

/*void	sig_non_interactive_heredoc(void)
{
	ft_printf("signal in sig_non_interactive_heredoc\n");
	rl_catch_signals = 0;
	signal(SIGINT, display_new_line_heredoc);
	signal(SIGQUIT, display_new_line_heredoc);
}*/

void	sig_interactive_heredoc(void)
{
	//ft_printf("signal in sig_interactive_heredoc\n");
	//rl_catch_signals = 0;
	signal(SIGINT, signal_handler_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_basic(void)
{
	//ft_printf("signal in sig_interactive\n");
	//rl_catch_signals = 0;
	signal(SIGINT, reset_line);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_execution(void)
{
	//rl_catch_signals = 0;
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
