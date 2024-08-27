#include "../../includes/minishell.h"

int	is_valid_number(char *arg)
{
	size_t	i = 0;

	if (arg[0] == '+' || arg[0] == '-')
	i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

void	handle_exit_status(char **command_words, t_global *global)
{
	if (command_words[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		global->exit_status = 1;
		return;
	}
	if (!is_valid_number(command_words[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(command_words[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		global->exit_status = 255;
		return;
	}
	global->exit_status = ft_atoi(command_words[1]);
}

void	cleanup_and_exit(t_global *global)
{
	rl_clear_history();
	free_env_list(&global->env_list);
	free_array(global->env);
	free(global);
	exit(global->exit_status);
}

void	exit_cmd(char **command_words, t_global *global)
{
	if (command_words[1])
	{
		handle_exit_status(command_words, global);
		if (command_words[2] || global->exit_status == 255)
			return;
	}
	else
		global->exit_status = 0;
	ft_putstr_fd("exit\n", 1);
	cleanup_and_exit(global);
}