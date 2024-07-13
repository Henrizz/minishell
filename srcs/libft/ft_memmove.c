/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:02:04 by Henriette         #+#    #+#             */
/*   Updated: 2023/11/23 15:10:27 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (src > dst)
	{
		while (i < len)
		{
			((char *)dst)[i] = ((char *)src)[i];
			i++;
		}
	}
	if (src < dst)
	{
		i = len -1;
		j = 0;
		while (j < len)
		{
			((char *)dst)[i - j] = ((char *)src)[i - j];
			j++;
		}
	}
	return (dst);
}
/*
#include <string.h>
#include <stdio.h>
int	main(void)
{
	const char	source[50] = "";
	char	dest[50] = "old string";
	const char	source2[50] = "";
	char	dest2[50] = "old string";
	
	printf("Before ft_memmove dest = %s, src = %s\n", dest, source);
	ft_memmove(dest, source, 9);
	printf("After ft_memmove dest = %s, src = %s\n", dest, source);
	printf("Before memmove dest = %s, src = %s\n", dest2, source2);
	memmove(dest2, source2, 9);
	printf("After memmove dest = %s, src = %s\n", dest2, source2);
	return (0);
}*/
