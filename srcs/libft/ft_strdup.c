/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 14:43:39 by hzimmerm          #+#    #+#             */
/*   Updated: 2023/11/19 16:58:36 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*dup;
	int		i;
	int		length;

	length = 0;
	i = 0;
	while (s[length] != 0)
	{
		length++;
	}
	dup = (char *)malloc(sizeof(char) * (length + 1));
	if (dup == 0)
		return (NULL);
	while (i < length)
	{
		dup[i] = s[i];
		i++;
	}
	dup[length] = 0;
	return (dup);
}
/*
#include <string.h>
#include <stdio.h>
int	main(void)
{
	char	*source;
	char	*duplicate;

	source = "Hello, World";
	duplicate = ft_strdup(source);
	if (duplicate != 0)
	{
		printf("imitated function: %s\n", source);
		printf("imitated function: %s\n", duplicate);
		free(duplicate);
	}
	duplicate = strdup(source);
	if (duplicate != 0)
	{
		printf("original function: %s\n", source);
        	printf("original function: %s\n", duplicate);
		free(duplicate);
	}
	return (0);
}*/
