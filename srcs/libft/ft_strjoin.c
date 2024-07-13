/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 09:37:38 by Henriette         #+#    #+#             */
/*   Updated: 2023/11/23 15:03:29 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new = malloc((ft_strlen(s1) + ft_strlen(s2)) * sizeof(char const) + 1);
	if (new == 0)
		return (0);
	while (s1[i] != 0)
	{
		new[j] = s1[i];
		i++;
		j++;
	}
	i = 0;
	while (s2[i] != 0)
	{
		new[j] = s2[i];
		i++;
		j++;
	}
	new[j] = 0;
	return (new);
}
/*
#include <stdio.h>
int	main(void)
{
	char	string1[] = "";
	char	string2[] = "der zweite ist hier";
	char	*ptr;
	ptr = ft_strjoin(string1, string2);
	printf("%s\n", ptr);
	free(ptr);
	return (0);
}*/
