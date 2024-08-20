#include "../../includes/minishell.h"

void print_array(char **array)
{
	int i;

	i = 0;
	while (array[i])
	{
		printf("%s\n", array[i]);
		i++;
	}
}

void append_new_var(t_env **env_list, t_env *new_var)
{
	t_env *last;

	if(*env_list == NULL)
		*env_list = new_var;
	else
	{
		last = *env_list;
		while(last->next != NULL)
			last = last->next;
		last->next = new_var;
	}
}