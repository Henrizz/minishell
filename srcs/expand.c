#include "../includes/minishell.h"

char *extract_var_name(const char *str, int i)
{
	int	j;
	char	*var_name;

	if (str == NULL || i < 0 || str[i] == '\0')
		return NULL;
	j = i;
	while (ft_isalnum(str[i]) || str[i] == '_')
		i++;
	var_name = ft_substr(str, j, i - j);
	if (var_name == NULL)
		return NULL;
	return var_name;
}

char	*get_value(char *var_name, t_env *env_list)
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

size_t	calcu_expanded_len(char *str, t_env *env_list)
{
	size_t expanded_len;
	int i;
	char *var_name;
	char *value;

	expanded_len = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
		{
			i++;
			var_name = extract_var_name(str, i);
			if (!var_name)
				return 0;
			value = get_value(var_name, env_list);
			if (!value)
				return 0;
			expanded_len += ft_strlen(value);
			i += ft_strlen(var_name);
			free(var_name);
		}
		else
		{
			expanded_len++;
			i++;
		}
	}
	return (expanded_len);
}

char	*expand_str(size_t expanded_len, char *str, t_env *env_list)
{
	int i;
	int k;
	char *var_name;
	char *value;
	char *expanded;

	i = 0;
	k = 0;
	expanded = (char *)malloc(expanded_len + 1);
	if (!expanded)
		return NULL;
	while (str[i])
	{
		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
		{
			i++;
			var_name = extract_var_name(str, i);
			if (!var_name)
				return (free(expanded), NULL);
			value = get_value(var_name, env_list);
			if (!value)
				return (free(expanded), NULL);
			while (*value)
				expanded[k++] = *value++;
			//free(var_name);
			i += ft_strlen(var_name) ;
		}
		else
			expanded[k++] = str[i++];
	}
	expanded[k] = '\0';
	return (expanded);
}

char	*expanding_var(char *str, t_env *env_list)
{
	size_t	expanded_len;
	char	*expanded;

	expanded_len = calcu_expanded_len(str, env_list);
	expanded = expand_str(expanded_len, str, env_list);
	if (!expanded)
		return (NULL);
	return (expanded);
}

char	*handle_quoting(char *str, t_env *env_list)
{
	size_t len;
	char *quote_free;
	char *temp;

	len = ft_strlen(str);
	quote_free = ft_strdup(str);
	if (str[0] == '\'' && str[len - 1] == '\'')
		temp = ft_substr(str, 1, ft_strlen(str) - 2);
    else if (str[0] == '"' && str[len - 1] == '"')
	{
		temp = ft_substr(str, 1, ft_strlen(str) - 2);
		temp = expanding_var(temp, env_list);
	}
    else
		temp = expanding_var(quote_free, env_list);
	if (temp)
	{
		free(quote_free);
		quote_free = temp;
	}
	return (quote_free);
}

void	expand_var_words(t_input *input, t_env *env_list)
{
	int	i;

	i = 0;
	while (input->words[i])
	{
		input->words[i] = handle_quoting(input->words[i], env_list);
		//printf("words[%d]: %s\n", i, input->words[i]);
		i++;
	}
}