/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 14:57:58 by Henriette         #+#    #+#             */
/*   Updated: 2023/11/24 17:47:54 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	len;

	len = ft_strlen(s);
	while (len > 0)
	{
		if (s[len] == (char)c)
			return ((char *)s + len);
		len--;
	}
	if (s[len] == (char)c)
		return ((char *)s + len);
	return (0);
}
/*
#include <stdio.h>
#include <string.h>
int main(void)
{
	char	source[] = "Hallo das ist ein string";
	int	arg = 'i';
	printf("string from found: %s\n", ft_strrchr(source, arg));
	printf("string from found: %s\n", strrchr(source, arg));
	printf("address:%p\n", ft_strrchr(source, arg));
	printf("address orig: %p\n", strrchr(source, arg));
	return (0);
}*/
