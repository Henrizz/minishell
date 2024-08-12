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

int syntax_error(char *str)
{
	int i;

	i = 0;
	if (str == NULL || str[0] == '\0')
		return (1);
	if (ft_isdigit(str[0]))
		return (1);
	while (str[i] != '\0')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
		{
			if (str[i] == '=' && i > 0)
				return (0);
			else
				return (1);
		}
		i++;
	}
	return (0);
}

void export(char **words, t_env *env_list)
{
	t_env	*current;
	char	*key;
	char	*value;

	current = env_list;
	key = NULL;
	value = NULL;
	if (!words[1])
	{
		while (current != NULL)
		{
			if((current->export == 1 || current->export == 2) &&
			!current->value[0])
				printf("declare -x %s\n", current->key);
			else if (current->export == 1 || current->export == 2)
			{
				printf("declare -x %s=\"%s\"\n", current->key, current->value);
			}
				current = current->next;
				}
	}
	else
	{
		if(syntax_error(words[1]))//Arreglar para cuando la variable fue expanded
		{
			printf("minishell: export: `%s': not a valid identifier\n", words[1]);
			return;
		}
        if (ft_strchr(words[1], '='))
        {
            key = ft_substr(words[1], 0, ft_strchr(words[1], '=') - words[1]);
            value = ft_substr(words[1], ft_strchr(words[1], '=') - words[1] + 1, ft_strlen(words[1]));
            if (!key || !value)
                return(free(key), free(value));  // Handle error
            set_env(key, value, env_list, 1); // con igual se deberia mostrar en env?!!
        }
        else
            set_env(words[1], "", env_list, 2);
        free(key);
        free(value);
    }
}

// void	export(char **words, t_env *env_list)
// {
// 	t_env	*current;
// 	char	*key;
// 	char	*value;

// 	current = env_list;
// 	key = NULL;
// 	value = NULL;
// 	if (!words[1])
// 	{printf("from IFFFF words[1]: %s\n", words[1]);
// 		while (current != NULL)
// 		{
// 			if (current->export)
// 				printf("declare -x %s=\"%s\"\n", current->key, current->value);
// 			current = current->next;
// 		}
// 	} else
// 	{
//         if (ft_strchr(words[1], '='))
// 		{
// 			printf("from if words[1]: %s\n", words[1]);
//             // If there's an equal sign, split key and value
// 			key = ft_substr(words[1], 0, ft_strchr(words[1], '=') - words[1]);
// 			value = ft_substr(words[1], ft_strchr(words[1], '=') - words[1] + 1, ft_strlen(words[1]));
//             set_env(key, value, env_list, 1);

//         } else
// 		{
// 			printf("from else words[1]: %s\n", words[1]);
//             // If no equal sign, just mark the variable as exported
// 			set_env(words[1], "", env_list, 1);
// 	}
// 	free(key);
// 	free(value);
// 	}
// }