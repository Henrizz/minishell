/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_altered.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 15:44:52 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/07/06 15:47:00 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_altered(char *s1, char *s2)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new = ft_calloc((ft_strlen(s1) + ft_strlen(s2)) + 1, sizeof(char));
	if (new == 0)
	{
		free(s1);
		return (0);
	}
	while (s1[i] != 0)
		new[j++] = s1[i++];
	i = 0;
	while (s2[i] != 0)
	{
		new[j] = s2[i];
		i++;
		j++;
	}
	new[j] = 0;
	free(s1);
	return (new);
}
