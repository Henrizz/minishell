#include "../../includes/minishell.h"

static size_t handle_tilde(int *i, t_global *global) {
    char *value = get_env_value("HOME", global->env_list);
    if (!value) return 0;

    size_t len = ft_strlen(value);
    free(value); // Free value after using it
    (*i)++;
    return len;
}

static size_t handle_exit_status(int *i, t_global *global) {
    char *itoa_str = ft_itoa(global->exit_status);
    if (!itoa_str) return 0;

    size_t len = ft_strlen(itoa_str);
    free(itoa_str); // Free itoa_str after using it
    (*i) += 2;
    return len;
}

static size_t handle_env_var(int *i, char *str, t_global *global) {
    (*i)++;
    char *var_name = extract_var_name(str, *i);
    if (!var_name) return 0;

    char *value = get_env_value(var_name, global->env_list);
    free(var_name); // Free var_name after using it
    if (!value) return 0;

    size_t len = ft_strlen(value);
    *i += ft_strlen(var_name);
    free(value); // Free value after using it
    return len;
}

size_t calc_expanded_len(char *str, t_global *global) {
    size_t expanded_len = 0;
    int i = 0;

    while (str[i]) {
        if (str[i] == '~') {
            expanded_len += handle_tilde(&i, global);
        } else if (str[i] == '$' && str[i + 1] == '?') {
            expanded_len += handle_exit_status(&i, global);
        } else if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_')) {
            expanded_len += handle_env_var(&i, str, global);
        } else {
            expanded_len++;
            i++;
        }
    }
    return expanded_len;
}




/*size_t calc_expanded_len(char *str, t_global *global) {
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
}*/

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