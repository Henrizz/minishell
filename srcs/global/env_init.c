
#include "../../includes/minishell.h"

void set_env(char *key, char *value, t_env *env_list, int export_flag)
{
    t_env *current = env_list;
    t_env *prev = NULL;
    size_t key_len;

    key_len = ft_strlen(key);
    while (current)
    {
        // Check if the key already exists
        if (!ft_strncmp(current->key, key, key_len) && ft_strlen(current->key) == key_len)
        {
            // Update the value of the existing key
            free(current->value);
            current->value = ft_strdup(value);
            if (!current->value)
                return;  // Handle error
            current->export = export_flag; // Update export flag if necessary
            return;
        }
        prev = current;
        current = current->next;
    }

    // Key doesn't exist, create a new variable
    t_env *new_var = new_env_var(key, export_flag);
    if (!new_var)
        return;  // Handle error

    // Assign the value to the new variable
    free(new_var->value); // Free the value allocated by new_env_var
    new_var->value = ft_strdup(value);
    if (!new_var->value)
    {
        free_env_var(new_var);  // Clean up in case of error
        return;
    }

    // Add the new variable to the end of the list
    if (prev)
        prev->next = new_var;
    else
        env_list = new_var;  // If the list was empty
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

// Sets the key and value for the t_env struct based on the input string
int	set_key_value(t_env *env_var, char *str)
{
	char	*equal_sign;
	size_t	len;

	equal_sign = ft_strchr(str, '=');
	len = ft_strlen(str);
	if (equal_sign)
	{
		env_var->key = ft_substr(str, 0, equal_sign - str);
		env_var->value = ft_substr(str, equal_sign - str + 1, len - (equal_sign - str + 1));
	}
	else
	{
		env_var->key = ft_strdup(str);
		env_var->value = ft_strdup(""); //TODO IMPORTANTE REVISAR
	}
	return (env_var->key && env_var->value);
}

// Creates a new environment variable
t_env	*new_env_var(char *str, int export)
{
	t_env	*new_var;

	new_var = allocate_env_var();
	if (!new_var)
		return (NULL);
	if (!set_key_value(new_var, str))
	{
		free_env_var(new_var);
		return (NULL);
	}
	new_var->export = export;
	return (new_var);
}

void	env_init(char **env, t_env **env_list)
{
	t_env	*new_var;

	while (*env)
	{
		new_var = new_env_var(*env, 1);
		if (!new_var)
		{
			free_env_list(env_list);
			exit_shell("env_init: failure to initialize env_list", EXIT_FAILURE);
			return;
		}
		append_new_var(env_list, new_var);
		env++;
	}
}
