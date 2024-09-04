#include "../../includes/minishell.h"

size_t calc_expanded_len(char *str, t_global *global) { //  leaks
    size_t expanded_len = 0;
    int i = 0;
    char *var_name = NULL;
    char *value = NULL;
    char *itoa_str;

    while (str[i]) {
        if (str[i] == '~') {
            // Handle '~' for HOME directory
                value = get_env_value("HOME", global->env_list);
                if (!value)
                    return 0;
                expanded_len += ft_strlen(value);
                free(value); // Ensure you free value after using it
                i++;

        } else if (str[i] == '$' && str[i + 1] == '?') {
            // Handle '$?'
            itoa_str = ft_itoa(global->exit_status);
            if (!itoa_str)
                return 0;
            expanded_len += ft_strlen(itoa_str);
            free(itoa_str); // Ensure you free itoa_str after using it
            i += 2;
        } else if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_')) {
            // Handle environment variables like $VAR
            i++;
            var_name = extract_var_name(str, i);
            if (!var_name)
                return 0;
            value = get_env_value(var_name, global->env_list);
            if (!value) {
                free(var_name);
                return 0;
            }
            expanded_len += ft_strlen(value);
            i += ft_strlen(var_name);
            free(var_name);
            free(value); // Ensure you free value after using it
        } else {
            expanded_len++;
            i++;
        }
    }
    return expanded_len;
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