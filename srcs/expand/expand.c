#include "../../includes/minishell.h"

char *expand_str(size_t expanded_len, char *str, t_global *global) { //leaks
    int i = 0;
    int k = 0;
    char *var_name = NULL;
    char *value = NULL;
    char *expanded = (char *)malloc(expanded_len + 1);
    char *temp_value;

    if (!expanded)
        return NULL;

    while (str[i]) {
        if (str[i] == '~') {
            // Handle '~' for HOME directory
            value = get_env_value("HOME", global->env_list);
            if (!value)
			{
                free(expanded);
                return NULL;
            }
            temp_value = value;
            while (*value)
                expanded[k++] = *value++;
            free(temp_value);
            i++;

        } else if (str[i] == '$' && str[i + 1] == '?') {
            // Handle '$?'
            value = ft_itoa(global->exit_status);
            if (!value) {
                free(expanded);
                return NULL;
            }
            temp_value = value;
            while (*value)
                expanded[k++] = *value++;
            free(temp_value);
            i += 2;
        } else if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_')) {
            // Handle environment variables like $VAR
            i++;
            var_name = extract_var_name(str, i);
            if (!var_name) {
                free(expanded);
                return NULL;
            }
            value = get_env_value(var_name, global->env_list);
            if (!value) {
                free(expanded);
                free(var_name);
                return NULL;
            }
            while (*value)
                expanded[k++] = *value++;
            i += ft_strlen(var_name);
            free(var_name);
            //free(value); // Ensure you free value after using it
        } else {
            expanded[k++] = str[i++];
        }
    }
    expanded[k] = '\0';
    return expanded;
}

/*char	*expand_str(size_t expanded_len, char *str, t_global *global) // no leaks
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
			 value = ft_itoa(global->exit_status);
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
			value = get_env_value(var_name, global->env_list);
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
}*/

// char *expand_str(size_t expanded_len, const char *str, t_global *global) {
//     size_t i = 0, k = 0;
//     char *var_name = NULL;
//     char *value = NULL;
//     char *expanded = (char *)malloc(expanded_len + 1);

//     if (!expanded) {
//         return NULL;
//     }

//     while (str[i]) {
//         if (str[i] == '~') {
//             if (str[i + 1] == '\0' || str[i + 1] == '/') {
//                 value = get_env_value("HOME", global->env_list);
//                 if (!value) {
//                     free(expanded);
//                     return NULL;
//                 }
//                 size_t len = ft_strlen(value);
//                 memcpy(expanded + k, value, len);
//                 k += len;
//                 free(value);  // Free the value after copying
//                 i++;
//             } else {
//                 expanded[k++] = str[i++];
//             }
//         } else if (str[i] == '$' && str[i + 1] == '?') {
//             value = ft_itoa(global->exit_status);
//             if (!value) {
//                 free(expanded);
//                 return NULL;
//             }
//             size_t len = ft_strlen(value);
//             memcpy(expanded + k, value, len);
//             k += len;
//             free(value);  // Free the value after copying
//             i += 2;
//         } else if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_')) {
//             i++;
//             var_name = extract_var_name(str, i);
//             if (!var_name) {
//                 free(expanded);
//                 return NULL;
//             }
//             value = get_env_value(var_name, global->env_list);
//             free(var_name);  // Free the var_name after use
//             if (!value) {
//                 free(expanded);
//                 return NULL;
//             }
//             size_t len = ft_strlen(value);
//             memcpy(expanded + k, value, len);
//             k += len;
//             free(value);  // Free the value after copying
//             i += ft_strlen(var_name);
//         } else {
//             expanded[k++] = str[i++];
//         }
//     }
//     expanded[k] = '\0'; // Null-terminate the result
//     return expanded;
// }

// char	*expand_str(size_t expanded_len, char *str, t_global *global)
// {
// 	int i;
// 	int k;
// 	char *var_name;
// 	char *value;
// 	char *expanded;
// 	char *temp_value;

// 	i = 0;
// 	k = 0;
// 	expanded = (char *)malloc(expanded_len + 1);
// 	if (!expanded)
// 		return NULL;
// 	while (str[i])
// 	{
// 		if (str[i] == '~')
// 		{
// 			if (str[i + 1] == '\0' || str[i + 1] == '/')
// 			{
// 				value = get_env_value("HOME", global->env_list);
// 				if (!value)
// 					return (free(expanded), NULL);
// 				temp_value = value;
// 				while (*value)
// 					expanded[k++] = *value++;
// 				free(temp_value);
// 				i++;
// 			}
// 			//else
// 			// {
// 			// 	expanded[k++] = str[i++];
// 			// }
// 		}
// 		if (str[i] == '$' && str[i + 1] == '?')
// 		{
// 			 value = ft_itoa(global->exit_status);
//             if (!value)
//                 return (free(expanded), NULL);
//             temp_value = value;
//             while (*value)
//                 expanded[k++] = *value++;
//             free(temp_value);
//             i += 2;
// 		}
// 		else
// 		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
// 		{
// 			i++;
// 			var_name = extract_var_name(str, i);
// 			if (!var_name)
// 				return (free(expanded), NULL);
// 			value = get_env_value(var_name, global->env_list);
// 			if (!value)
// 				return (free(expanded), NULL);
// 			while (*value)
// 				expanded[k++] = *value++;
// 			i += ft_strlen(var_name);
// 			//free(value); //TODO
// 			free(var_name);
// 		}
// 		else
// 			expanded[k++] = str[i++];
// 	}
// 	expanded[k] = '\0';
// 	return (expanded);
// }

char	*expanding_var(char *str, t_global *global)
{
	size_t	expanded_len;
	char	*expanded;

	expanded_len = calc_expanded_len(str, global);
	expanded = expand_str(expanded_len, str, global);
	if (!expanded)
		return (NULL);
	return (expanded);
}

char	*concat_and_free(char *s1, char *s2)
{
    char *new_str;

	new_str = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
    if (!new_str)
	{
        free(s1);
        free(s2);
        return NULL;
    }
    ft_strlcpy(new_str, s1, ft_strlen(s1) + 1);
    ft_strlcat(new_str, s2, ft_strlen(s1) + ft_strlen(s2) + 1);
    free(s1);
    free(s2);
    return (new_str);
}

char	*process_quoted_segment(char **current, char quote, t_global *global)
{
	char *start;
	char *segment;
	char *expanded;

    start = ++(*current);
    while (**current != quote && **current != '\0')
        (*current)++;
    segment = ft_substr(start, 0, *current - start);
    if (quote == '"')
	{
        expanded = expanding_var(segment, global);
        free(segment);
        segment = expanded;
    }
    if (**current == quote)
        (*current)++;
    return (segment);
}

char	*process_non_quoted_segment(char **current, t_global *global)
{
    char *start;
	char *segment;
	char *expanded;

	start = *current;
    while (**current != '\'' && **current != '"' && **current != '\0')
        (*current)++;
    segment = ft_substr(start, 0, *current - start);
    expanded = expanding_var(segment, global);
    free(segment);
    return (expanded);
}

char	*handle_quoting(char *str, t_global *global)
{
    char	*result;
	char	*current;
	char	*segment;

	result = ft_strdup("");
    if (!result)
        return NULL;
    current = str;
    while (*current != '\0')
	{
        segment = NULL;
        if (*current == '\'' || *current == '"')
            segment = process_quoted_segment(&current, *current, global);
		else
            segment = process_non_quoted_segment(&current, global);
        if (!segment)
            return (free(result), NULL);
        result = concat_and_free(result, segment);
        if (!result)
            return (NULL);
    }
    return (free(str), result);
}


// char	*handle_quoting(char *str, t_env *env_list, int exit_status)
// {
// 	char	*quote_free;
// 	char	*temp;
// 	char	*expanded;

// 	quote_free = ft_strdup(str);
// 	if (str[0] == '\'' && str[ft_strlen(str) - 1] == '\'')
// 		temp = ft_substr(str, 1, ft_strlen(str) - 2);
// 	else if (str[0] == '"' && str[ft_strlen(str) - 1] == '"')
// 	{
// 		temp = ft_substr(str, 1, ft_strlen(str) - 2);
// 		if (temp)
// 		{
// 			expanded = expanding_var(temp, env_list, exit_status);
// 			free(temp);
// 			temp = expanded;
// 			}
// 	}
// 	else
// 		temp = expanding_var(quote_free, env_list, exit_status);
// 	if (temp)
// 	{
// 		free(quote_free);
// 		quote_free = temp;
// 	}
// 	free(str);
// 	return (quote_free);
// }


void	expand_var_words(t_input *input, t_global *global)
{
	int	i;
	t_input *temp;

	temp = input;
	while (temp)
	{
		i = 0;
		while (temp->words[i])
		{
			temp->words[i] = handle_quoting(temp->words[i], global);
			//printf("words[%d]: %s\n", i, input->words[i]);
			i++;
		}
		i = 0;
		while(temp->redirections[i])
		{
			temp->redirections[i] = handle_quoting(temp->redirections[i], global);
			i++;
		}
	/*while(input->red_in[i])
	{
		input->red_in[i] = handle_quoting(input->red_in[i], env_list, exit_status);
		i++;
	}
	i = 0;
	while(input->red_out[i])
	{
		input->red_out[i] = handle_quoting(input->red_out[i], env_list, exit_status);
		i++;
	}*/
	temp = temp->next;
	}
}
