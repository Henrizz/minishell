#include "../../includes/minishell.h"

// Function prototypes
char *handle_tilde(t_expand_state *state, t_global *global);
char *handle_exit_stat(t_expand_state *state, t_global *global);
char *handle_variable(t_expand_state *state, char *str, t_global *global);
char *append_char(t_expand_state *state, char c);
char *process_expansion(t_expand_state *state, char *str, t_global *global);
char *expand_str(size_t expanded_len, char *str, t_global *global);

// Handle the tilde (~) expansion
char *handle_tilde(t_expand_state *state, t_global *global)
{
	char *temp_value;
    char *value = get_env_value("HOME", global->env_list);
    if (!value)
        return NULL;
	temp_value = value;
    while (*value)
        state->expanded[(state->k)++] = *value++;
    global->home_expanded = 1;
	free(temp_value);
	state->i++;
    return state->expanded;
}

// Handle the special variable $?
char *handle_exit_stat(t_expand_state *state, t_global *global) {
	char *temp_value;
    char *value = ft_itoa(global->exit_status);
    if (!value)
        return NULL;
	temp_value = value;
    while (*value)
        state->expanded[(state->k)++] = *value++;
    free(temp_value);
	state->i +=2;
    return state->expanded;
}

// Handle environment variable expansion
char *handle_variable(t_expand_state *state, char *str, t_global *global) {
	char *temp_value;
    char *var_name = extract_var_name(str, (++state->i));
    if (!var_name)
        return NULL;
    char *value = get_env_value(var_name, global->env_list);

    if (!value)
        return NULL;
	temp_value = value;
    while (*value)
        state->expanded[(state->k)++] = *value++;
    state->i += ft_strlen(var_name);
	free(var_name);
    free(temp_value);
    return state->expanded;
}

// Append a single character to the expanded string
char *append_char(t_expand_state *state, char c) {
    state->expanded[(state->k)++] = c;
    return state->expanded;
}

// Process each character for potential expansion
char *process_expansion(t_expand_state *state, char *str, t_global *global) {
    if (str[state->i] == '~') {
        return handle_tilde(state, global);
    } else if (str[state->i] == '$' && str[state->i + 1] == '?') {
        state->i++;
        return handle_exit_stat(state, global);
    } else if (str[state->i] == '$' && (ft_isalnum(str[state->i + 1]) || str[state->i + 1] == '_')) {
        return handle_variable(state, str, global);
    }
    return append_char(state, str[(state->i)++]);
}

// Main function that orchestrates the string expansion
char *expand_str(size_t expanded_len, char *str, t_global *global) {
    t_expand_state state;
    state.i = 0;
    state.k = 0;
    state.expanded = (char *)malloc(expanded_len + 1);

    if (!state.expanded)
        return NULL;

    while (str[state.i]) {
        if (!(state.expanded = process_expansion(&state, str, global))) {
            free(state.expanded);
            return NULL;
        }
    }

    state.expanded[state.k] = '\0';
    return state.expanded;
}


/*char *expand_str(size_t expanded_len, char *str, t_global *global)
{
	t_expand_state state;

    state.i = 0;
    state.k = 0;
    state.expanded = (char *)malloc(expanded_len + 1);
    char *var_name = NULL;
    char *value = NULL;
    char *temp_value;

    if (!state.expanded)
        return NULL;

    while (str[i]) {
        if (str[i] == '~') {
            // Handle '~' for HOME directory
            value = get_env_value("HOME", global->env_list);
            if (!value)
			{
                free(state.expanded);
                return NULL;
            }
            temp_value = value;
            while (*value)
                state.expanded[k++] = *value++;
			global->home_expanded = 1;
            free(temp_value);
            i++;

        } else if (str[i] == '$' && str[i + 1] == '?') {
            // Handle '$?'
            value = ft_itoa(global->exit_status);
            if (!value) {
                free(state.expanded);
                return NULL;
            }
            temp_value = value;
            while (*value)
                state.expanded[k++] = *value++;
            free(temp_value);
            i += 2;
        } else if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_')) {
            // Handle environment variables like $VAR
            i++;
            var_name = extract_var_name(str, i);
            if (!var_name) {
                free(state.expanded);
                return NULL;
            }
            value = get_env_value(var_name, global->env_list);
            if (!value) {
                free(state.expanded);
                free(var_name);
                return NULL;
            }
            temp_value = value;
            while (*value)
                state.expanded[k++] = *value++;
            i += ft_strlen(var_name);
            free(var_name);
            free(temp_value); // Ensure you free value after using it
        } else {
            state.expanded[k++] = str[i++];
        }
    }
    state.expanded[k] = '\0';
    return state.expanded;
}*/

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

void	expand_var_words(t_input *input, t_global *global)
{
	int	i;
	t_input *temp;

	temp = input;
	global->home_expanded = 0;
	while (temp)
	{
		i = 0;
		while (temp->words[i])
		{
			temp->words[i] = handle_quoting(temp->words[i], global);
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
