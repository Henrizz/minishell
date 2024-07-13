/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 11:21:30 by Henriette         #+#    #+#             */
/*   Updated: 2023/11/24 15:37:28 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (dst == 0 && src == 0)
		return (0);
	while (i < n)
	{
		((char *)dst)[i] = ((char *)src)[i];
		i++;
	}
	return (dst);
}
/*
#include <string.h>
#include <stdio.h>
int	main(void)
{
	const char	source[50] = "";
	char	dest[50];
	char	dest2[50];
	//strcpy(dest, "Hallo das ist die erste Kopie");
	printf("original dest: %s\n", dest);
	ft_memcpy(dest, 0, 0);
	printf("modified with ft_memcpy: %s\n", dest);
	//strcpy(dest2, "Hallo das ist die erste Kopie");
	memcpy(dest2, source, 0);
	printf("modified with memcpy: %s\n", dest2);
	return (0);
}*/
