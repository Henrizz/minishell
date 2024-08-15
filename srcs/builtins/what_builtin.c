#include "../../includes/minishell.h"

void	what_builtin(char **command_words, t_global *global)
{
	if (!command_words[0])
		return;
	if(!ft_strncmp(command_words[0], "echo", 4))
	 	echo(command_words);
	if(!ft_strncmp(command_words[0], "cd", 2))
	 	cd(command_words[1], global->env_list, &global->env);
	if(!ft_strncmp(command_words[0], "pwd", 3))
		pwd();
	if(!ft_strncmp(command_words[0], "export", 6))
		export(command_words, global->env_list, &global->env);
	if(!ft_strncmp(command_words[0], "unset", 5))
		unset(command_words, global->env_list, &global->env);
	if(!ft_strncmp(command_words[0], "env", 3))
	 	cmd_env(global->env_list, command_words);
	// if(!ft_strncmp(command_words[0], "exit", 4))
	// 	exit();
}
