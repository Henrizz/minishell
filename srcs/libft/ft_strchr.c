/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:36:38 by Henriette         #+#    #+#             */
/*   Updated: 2023/11/23 15:03:04 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strchr(const char *s, int c)
{
	while (*s != 0)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == 0)
		return ((char *)s);
	return (0);
}
/*
#include <stdio.h>
#include <string.h>
int main(void)
{
	char	source[] = "Hallo das ist ein string";
	int	arg = 0;
	printf("string from found: %s\n", ft_strchr(source, arg));
	printf("string from found: %s\n", strchr(source, arg));
	printf("address:%p\n", ft_strchr(source, arg));
	printf("address orig: %p\n", strchr(source, arg));
	return (0);
}*/
