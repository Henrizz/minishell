#include "../../includes/minishell.h"

void	what_builtin(char *command_name, t_env *env_list)
{
    size_t len;

    len = ft_strlen(command_name);
    // if(!ft_strncmp(command_name, "echo", len))
	// 	echo();
	// if(!ft_strncmp(command_name, "cd", len))
	// 	cd(path);
	if(!ft_strncmp(command_name, "pwd", len))
		pwd();
	// if(!ft_strncmp(command_name, "export", len))
	// 	export();
	// if(!ft_strncmp(command_name, "unset", len))
	// 	unset();
	if(!ft_strncmp(command_name, "env", len))
	 	cmd_env(env_list);
	// if(!ft_strncmp(command_name, "exit", len))
	// 	exit();
}