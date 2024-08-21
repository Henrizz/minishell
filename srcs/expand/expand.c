#include "../../includes/minishell.h"

char	*expand_str(size_t expanded_len, char *str, t_env *env_list, int exit_status)
{
	int i;
	int k;
	char *var_name;
	char *value;
	char *expanded;
	char *temp_value;

	i = 0;
	k = 0;
	expanded = (char *)malloc(expanded_len + 1);
	if (!expanded)
		return NULL;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
		{
			 value = ft_itoa(exit_status);
            if (!value)
                return (free(expanded), NULL);
            temp_value = value;
            while (*value)
                expanded[k++] = *value++;
            free(temp_value);
            i += 2;
		}
		else
		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
		{
			i++;
			var_name = extract_var_name(str, i);
			if (!var_name)
				return (free(expanded), NULL);
			value = get_env_value(var_name, env_list);
			if (!value)
				return (free(expanded), NULL);
			while (*value)
				expanded[k++] = *value++;
			i += ft_strlen(var_name);
			free(var_name);
		}
		else
			expanded[k++] = str[i++];
	}
	expanded[k] = '\0';
	return (expanded);
}

char	*expanding_var(char *str, t_env *env_list, int exit_status)
{
	size_t	expanded_len;
	char	*expanded;

	expanded_len = calc_expanded_len(str, env_list, exit_status);
	expanded = expand_str(expanded_len, str, env_list, exit_status);
	if (!expanded)
		return (NULL);
	return (expanded);
}

char	*handle_quoting(char *str, t_env *env_list, int exit_status)
{
	char	*quote_free;
	char	*temp;
	char	*expanded;

	quote_free = ft_strdup(str);
	if (str[0] == '\'' && str[ft_strlen(str) - 1] == '\'')
		temp = ft_substr(str, 1, ft_strlen(str) - 2);
	else if (str[0] == '"' && str[ft_strlen(str) - 1] == '"')
	{
		temp = ft_substr(str, 1, ft_strlen(str) - 2);
		if (temp)
		{
			expanded = expanding_var(temp, env_list, exit_status);
			free(temp);
			temp = expanded;
			}
	}
	else
		temp = expanding_var(quote_free, env_list, exit_status);
	if (temp)
	{
		free(quote_free);
		quote_free = temp;
	}
	free(str);
	return (quote_free);
}

void	expand_var_words(t_input *input, t_env *env_list, int exit_status)
{
	int	i;

	i = 0;
	while (input->words[i])
	{
		input->words[i] = handle_quoting(input->words[i], env_list, exit_status);
		//printf("words[%d]: %s\n", i, input->words[i]);
		i++;
	}
}