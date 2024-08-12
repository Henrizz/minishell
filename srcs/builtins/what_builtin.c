#include "../../includes/minishell.h"

void	what_builtin(char **command_words, t_env *env_list)
{
    size_t len;

    if (!command_words[0])
		return;
    len = ft_strlen(command_words[0]);
    if(!ft_strncmp(command_words[0], "echo", len))
	 	echo(command_words);
	if(!ft_strncmp(command_words[0], "cd", len))
	 	cd(command_words[1], env_list);
	if(!ft_strncmp(command_words[0], "pwd", len))
		pwd();
	if(!ft_strncmp(command_words[0], "export", len))
		export(command_words, env_list);
	// if(!ft_strncmp(command_words, "unset", len))
	// 	unset();
	if(!ft_strncmp(command_words[0], "env", len))
	 	cmd_env(env_list, command_words);
	// if(!ft_strncmp(command_words, "exit", len))
	// 	exit();
}
