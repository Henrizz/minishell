
#include "../../includes/minishell.h"

void	cmd_env(t_env *list)
{
	t_env *temp = list;

	while(temp)
	{
		printf("%s=%s\n", temp->key, temp->value);
		temp = temp->next;
	}
}	