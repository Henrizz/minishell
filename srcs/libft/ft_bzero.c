/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 12:59:53 by hzimmerm          #+#    #+#             */
/*   Updated: 2023/11/19 16:30:13 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*ptr;

	i = 0;
	ptr = (char *)s;
	while (i < n)
	{
		ptr[i] = 0;
		i++;
	}
}
/*
#include <stdio.h>
#include <string.h>
int	main(void)
{
	char string1[] = "Hallo";
	char *ptr1;
	char string2[] = "Hallo";
	char *ptr2;
	
	ptr1 = string1;
	ptr2 = string2;
	ft_bzero(ptr1, 4);
	printf("%s\n", ptr1);
	bzero(ptr2, 4);
	printf("%01x\n", ptr2[0]);
	return (0);
}*/
