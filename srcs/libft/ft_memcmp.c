/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 16:07:19 by Henriette         #+#    #+#             */
/*   Updated: 2023/11/23 15:11:03 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (((unsigned char *)s1)[i] != ((unsigned char *)s2)[i])
			return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
		i++;
	}
	return (0);
}
/*
#include <string.h>
#include <stdio.h>
int main(void)
{
    char    string1[] = "";
    char    string2[] = "";
    char    *ptr1;
    char    *ptr2;

    ptr1 = string1;
    ptr2 = string2;
    printf("%d\n", ft_memcmp(ptr1, ptr2, 10));
    printf("%d\n", memcmp(ptr1, ptr2, 10));
    return (0);
}*/
