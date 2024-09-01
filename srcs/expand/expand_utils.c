#include "../../includes/minishell.h"

size_t	calc_expanded_len(char *str, t_env *env_list, int exit_status)
{
	size_t expanded_len;
	int i;
	char *var_name;
	char *value;
	char *itoa_str;

	expanded_len = 0;
	i = 0;
	while (str[i])
	{
		if(str[i] == '$' && str[i + 1] == '?')
		{
			 itoa_str = ft_itoa(exit_status);
            if (!itoa_str)
                return 0;
            expanded_len += ft_strlen(itoa_str);
            free(itoa_str);
            i += 2;
		}
		else
		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
		{
			i++;
			var_name = extract_var_name(str, i);
			if (!var_name)
				return 0;
			value = get_env_value(var_name, env_list);
			if (!value)
				return 0;
			expanded_len += ft_strlen(value);
			i += ft_strlen(var_name);
			//free(value); //TODO
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
	{
		return (free(var_name), NULL);
	}
	return var_name;
}