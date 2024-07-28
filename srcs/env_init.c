
#include "../includes/minishell.h"

static void	append_var(t_env **env_list, char *env)
{
	t_env *new_var;
	t_env *last;
	size_t len;

	len = ft_strlen(env);
	new_var = malloc(sizeof(t_env));
	if(!new_var)
		return;
	new_var->key = ft_substr(env, 0, ft_strchr(env, '=') - env); //malloc
	new_var->value = ft_substr(env, ft_strchr(env, '=') - env + 1, len); //malloc
	new_var->next = NULL;
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

void	env_init(char **env, t_env **env_list)
{
	while(*env)
	{
        append_var(env_list, *env);
		env++;
	}
}

