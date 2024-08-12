
#include "../../includes/minishell.h"

void	cmd_env(t_env *list, char **command_words)
{
	t_env *temp = list;

	if(command_words[1])
	{
		printf("env: %s: No such file or directory\n", command_words[1]);
		return;
	}
	if(list == NULL)
		return;
	while(temp)
	{
		 if (temp->key && temp->value && temp->export == 1)
			printf("%s=%s\n", temp->key, temp->value);
		temp = temp->next;
	}
}