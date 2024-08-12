
#include "../includes/minishell.h"

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
			// Handle error
			return;
		}
		append_new_var(env_list, new_var);
		env++;
	}
}

// void set_env(char *key, char *value, t_env *env_list, int export_flag)
// {
//     t_env *current;

//     current = env_list;
//     while (current)
//     {
//         if (!ft_strncmp(current->key, key, ft_strlen(key)) && ft_strlen(current->key) == ft_strlen(key))
//         {
//             free(current->value);
//             current->value = ft_strdup(value);
//             if (!current->value)
//                 return;  // Handle error
//             return;
//         }
//         current = current->next;
//     }
//     current = malloc(sizeof(t_env));
//     if (!current)
//         return;  // Handle error
//     current->key = ft_strdup(key);
//     if (!current->key)
//     {
//         free(current);
//         return;  // Handle error
//     }
//     current->value = ft_strdup(value);
//     if (!current->value)
//     {
//         free(current->key);
//         free(current);
//         return;  // Handle error
//     }
//     current->export = export_flag;
//     current->next = NULL;

//     if (env_list == NULL)
//         env_list = current;
//     else
//     {
//         t_env *temp = env_list;
//         while (temp->next)
//             temp = temp->next;
//         temp->next = current;
//     }
// }

// static void	append_var(t_env **env_list, char *env)
// {
// 	t_env *new_var;
// 	t_env *last;
// 	size_t len;

// 	len = ft_strlen(env);
// 	new_var = malloc(sizeof(t_env));
// 	if(!new_var)
// 		return;
// 	new_var->key = ft_substr(env, 0, ft_strchr(env, '=') - env); //malloc
// 	new_var->value = ft_substr(env, ft_strchr(env, '=') - env + 1, len); //malloc
// 	new_var->export = 1;
// 	new_var->next = NULL;
// 	if(*env_list == NULL)
// 		*env_list = new_var;
// 	else
// 	{
// 		last = *env_list;
// 		while(last->next != NULL)
// 			last = last->next;
// 		last->next = new_var;
// 	}
// }