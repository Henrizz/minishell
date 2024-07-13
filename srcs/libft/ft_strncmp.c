/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 13:30:42 by hzimmerm          #+#    #+#             */
/*   Updated: 2023/11/19 16:18:41 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (s1[i] == 0 || s2[i] == 0 || s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++; 
	}
	return (0);
}
/*
#include <string.h>
#include <stdio.h>
int main(void)
{
    char    string1[] = "test";
    char    string2[] = "testss";
    char    *ptr1;
    char    *ptr2;

    ptr1 = string1;
    ptr2 = string2;
    printf("%d\n", ft_strncmp(ptr1, ptr2, 7));
    printf("%d\n", strncmp(ptr1, ptr2, 7));
    return (0);
}*/
