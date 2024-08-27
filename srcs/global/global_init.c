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

int populate_env_array(t_env *env_list, char **env_array)
{
	t_env	*current;
	int			i;
	char	*temp;

	current = env_list;
	i = 0;
	while (current)
	{
		env_array[i] = ft_strjoin(current->key, "=");
		if (!env_array[i])
			return (free_array(env_array), 0);
		temp = ft_strjoin(env_array[i], current->value);
		free(env_array[i]);
		if (!temp)
			return (free_array(env_array), 0);
		env_array[i++] = temp;
		current = current->next;
	}
	env_array[i] = NULL;
	return (1);
}

int	set_env_array(t_env *env_list, char ***env_array)
{
	t_env	*current;
	int		env_count;

	current = env_list;
	env_count = 0;
	while (current)
	{
		env_count++;
		current = current->next;
	}
	if (*env_array)
	{
		free_array(*env_array);
		*env_array = NULL;
	}
	*env_array = malloc(sizeof(char *) * (env_count + 1));
	if (!*env_array)
		return (0);
	if(populate_env_array(env_list, *env_array))
		return (1);
	return (0);
}

void	global_init(t_global **global, char **env)
{
	t_env *env_list;
	char **env_array;

	env_list = NULL;
	env_array = NULL;
	init_signals();
	*global = malloc(sizeof(t_global));
	if (!*global)
	{
		perror("minishell: malloc");
		exit(EXIT_FAILURE);
	}
	(*global)->exit_status = 0;
	(*global)->pwd = getenv("PWD");
	if(env_init(env, &env_list))
		(*global)->env_list = env_list;
	if(set_env_array((*global)->env_list, &env_array))
		(*global)->env = env_array;
	//print_array(env_array);
	make_history_file(global);
}
