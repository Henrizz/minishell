#include "../../includes/minishell.h"

// Frees allocated memory for the t_env struct in case of an error
void	free_env_var(t_env *env_var)
{
	if (env_var)
	{
		free(env_var->key);
		free(env_var->value);
		free(env_var);
	}
}

void	free_env_list(t_env **env_list)
{
	t_env	*current;
	t_env	*next;

	current = *env_list;
	while (current)
	{
		next = current->next;
		free_env_var(current);
		current = next;
	}
	*env_list = NULL;
}

t_env	*allocate_env_var(void)
{
	t_env	*new_var;

	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return (NULL);
	new_var->key = NULL;
	new_var->value = NULL;
	new_var->export = 0;
	new_var->next = NULL;
	return (new_var);
}

char	*get_env_value(char *var_name, t_env *env_list)
{
	t_env	*current;
	size_t len;

	current = env_list;
	len = ft_strlen(var_name);
	while (current)
	{
		if (ft_strncmp(var_name, current->key, len) == 0 && current->key[len] == '\0')
			return (current->value);
		current = current->next;
	}
	return ("");
}