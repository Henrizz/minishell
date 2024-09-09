/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redir_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:29:50 by smanriqu          #+#    #+#             */
/*   Updated: 2024/09/09 19:11:30 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*extract_segment(char **curr, char quote)
{
	char	*start;

	start = *curr;
	while (**curr != quote && **curr != '\0')
		(*curr)++;
	return (ft_substr(start, 0, *curr - start));
}

int	contains_dollar_sign(const char *str)
{
	return (ft_strchr(str, '$') != NULL);
}
