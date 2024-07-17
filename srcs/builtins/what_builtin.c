#include "../../includes/minishell.h"

void	what_builtin(char *function_name)
{
    size_t len;

    len = ft_strlen(function_name);
    // if(!ft_strncmp(function_name, "echo", len))
	// 	echo();
	// if(!ft_strncmp(function_name, "cd", len))
	// 	cd(path);
	if(!ft_strncmp(function_name, "pwd", len))
		pwd();
	// if(!ft_strncmp(function_name, "export", len))
	// 	export();
	// if(!ft_strncmp(function_name, "unset", len))
	// 	unset();
	// if(!ft_strncmp(function_name, "env", len))
	// 	env();
	// if(!ft_strncmp(function_name, "exit", len))
	// 	exit();
}