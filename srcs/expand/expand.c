#include "../../includes/minishell.h"

char	*process_expansion(t_expand_state *state, char *str, t_global *global)
{
	if (str[state->i] == '~')
		return (handle_home(state, global));
	else if (str[state->i] == '$' && str[state->i + 1] == '?')
		return (handle_exit(state, global));
	else if (str[state->i] == '$' && (ft_isalnum(str[state->i + 1]) || str[state->i + 1] == '_'))
		return (handle_var(state, str, global));
	state->expanded[(state->k)++] =  str[(state->i)++];
	return (state->expanded);
}

char	*expanding_var(char *str, t_global *global)
{
	size_t	expanded_len;
	t_expand_state	state;
	state.i = 0;
	state.k = 0;

	expanded_len = calc_expanded_len(str, global);
	state.expanded = (char *)malloc(expanded_len + 1);
	if (!state.expanded)
		return (NULL);
	while (str[state.i])
	{
		if (!(state.expanded = process_expansion(&state, str, global)))
		{
			free(state.expanded);
			return (NULL);
		}
	}
	state.expanded[state.k] = '\0';
	return (state.expanded);
}

char	*process_quoted_segment(char **current, char quote, t_global *global)
{
	char	*start;
	char	*segment;
	char	*expanded;

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
	char	*start;
	char	*segment;
	char	*expanded;

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
		return (NULL);
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
		temp = temp->next;
	}
}
