/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 09:17:01 by Henriette         #+#    #+#             */
/*   Updated: 2023/11/29 15:58:46 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	i;

	if (start >= ft_strlen(s))
	{
		sub = malloc(1);
		if (sub == 0)
			return (0);
		sub[0] = '\0';
		return (sub);
	}
	if (ft_strlen(s + start) < len)
		len = ft_strlen(s + start);
	i = 0;
	sub = malloc((len + 1) * sizeof(char));
	if (sub == 0)
		return (0);
	while (i < len && s[start] != 0)
	{
		sub[i] = s[start];
		start++;
		i++;
	}
	sub[i] = 0;
	return (sub);
}
/*
#include <stdio.h>
int	main(void)
{
	char	string[] = "hola";
	char	*ptr;
	ptr = ft_substr(string, 0, 18446744073709551615ULL);
	printf("%s\n", ptr);
	free(ptr);
	return (0);
}*/
