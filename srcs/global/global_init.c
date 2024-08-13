#include "../../includes/minishell.h"

char *ft_strjoin_free(char *s1, char *s2, int free_str)
{
	char *new_str;

	new_str = ft_strjoin(s1, s2);
	if (free_str == 1)
		free(s1);
	else if (free_str == 2)
		free(s2);
	else if (free_str == 3)
	{
		free(s1);
		free(s2);
	}
	return (new_str);
}

void free_env_array(char **array)
{
	int i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	set_env_array(t_env *env_list, char ***env_array)
{
	t_env	*current;
	int		i;

	current = env_list;
	i = 0;
	while (current)
	{
		i++;
		current = current->next;
	}
	if(!*env_array)
	{
		free_array(*env_array);
		*env_array = NULL;
	}
	*env_array = malloc(sizeof(char *) * (i + 1));
	if (!*env_array)
		exit_shell("Error: malloc failed\n", EXIT_FAILURE);
	i = 0;
	current = env_list;
	while (current)
	{
		(*env_array)[i] = ft_strjoin(current->key, "=");
		if (!(*env_array)[i])
			exit_shell("Error: malloc failed\n", EXIT_FAILURE);
		(*env_array)[i] = ft_strjoin_free((*env_array)[i], current->value, 1);
		if (!(*env_array)[i])
			exit_shell("Error: malloc failed\n", EXIT_FAILURE);
		i++;
		current = current->next;
	}
	(*env_array)[i] = NULL;
}

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

void	global_init(t_global **global, char **env)
{
	t_env *env_list;
	char **env_array;

	env_list = NULL;
	env_array = NULL;
	*global = malloc(sizeof(t_global));
	if (!*global)
		exit_shell("Error: malloc failed\n", EXIT_FAILURE);
	(*global)->exit_status = 0;
	(*global)->pwd = getenv("PWD");
	env_init(env, &env_list);
	(*global)->env_list = env_list;
	set_env_array((*global)->env_list, &env_array);
	(*global)->env = env_array;
	//print_array(env_array);
}