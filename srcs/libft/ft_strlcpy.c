/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 13:00:49 by hzimmerm          #+#    #+#             */
/*   Updated: 2023/11/19 18:04:27 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, char *src, size_t	size)
{
	size_t	i;

	i = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (i < size - 1 && src[i] != 0)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = 0;
	i = 0;
	return (ft_strlen(src));
}
/*
#include <string.h>
#include <stdio.h>
int	main(void)
{
	char buffer[10];
	char source[] = "Hallo das ist string";
	char source2[] = "Hallo das ist string";
	char *ptr1;
	char *ptr2;
	char *ptr3;
	ptr1 = source;
	ptr2 = buffer;
	ptr3 = source2;
	ft_strlcpy(ptr2, ptr1, 0);
	printf("%zu\n", ft_strlcpy(ptr2, ptr1, 0));
	//strlcpy(ptr2, ptr3, 5);
	//printf("%s\n", ptr2); 
	return (0);
}*/
