/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 15:26:26 by Henriette         #+#    #+#             */
/*   Updated: 2023/11/23 14:56:11 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (((char *)s)[i] == (char)c)
			return (&((char *)s)[i]);
		i++;
	}
	return (0);
}
/*
#include <stdio.h>
#include <string.h>
int main(void)
{
	char	source[] = "Hallo das ist ein string";
	int	arg = 'H';
	size_t	n = 0;
	printf("string from found: %s\n", ft_memchr(source, arg, n));
	printf("string from found orig: %s\n", memchr(source, arg, n));
	printf("address:%p\n", ft_memchr(source, arg, n));
	printf("address orig: %p\n", memchr(source, arg, n));
	return (0);
}*/
