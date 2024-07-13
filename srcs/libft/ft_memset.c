/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 13:00:30 by hzimmerm          #+#    #+#             */
/*   Updated: 2023/11/19 16:32:42 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	arg;
	size_t			i;
	char			*ptr;

	arg = c;
	i = 0;
	ptr = (char *)s;
	while (i < n)
	{
		ptr[i] = arg;
		i++;
	}
	return (s);
}
/*
#include <stdio.h>
#include <string.h>
int	main(void)
{
	char string1[] = "";
	char *ptr1;
	char string2[] = "";
	char *ptr2;
	
	ptr1 = string1;
	ptr2 = string2;
	ft_memset(ptr1, '#', 4);
	printf("%s\n", ptr1);
	memset(ptr2, '#', 4);
	printf("%s\n", ptr2);
	return (0);
}*/
