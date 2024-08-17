
#include "../../includes/minishell.h"

void	cmd_env(char **command_words, t_global *global)
{
	t_env *temp;

	temp = global->env_list;
	if(command_words[1])
	{
		printf("env: %s: No such file or directory\n", command_words[1]);
		global->exit_status = 1;
		return;
	}
	if(global->env_list == NULL)
	{
		global->exit_status = 0;
		return;
	}
	while(temp)
	{
		 if (temp->key && temp->value && temp->export == 1)
			printf("%s=%s\n", temp->key, temp->value);
		temp = temp->next;
	}
	global->exit_status = 0;
}