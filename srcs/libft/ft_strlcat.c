/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 13:00:44 by hzimmerm          #+#    #+#             */
/*   Updated: 2023/11/24 16:24:03 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i] != 0)
		i++;
	return (i);
}*/

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	dst_length;

	i = 0;
	j = 0;
	dst_length = ft_strlen(dst);
	if (size == 0)
		return (ft_strlen(src));
	while (dst[i] != 0 && i < size)
		i++;
	if (i == size)
		return (size + ft_strlen(src));
	while (j < (size - i - 1) && src[j] != 0)
	{
		dst[i + j] = src[j];
		j++;
	}
	dst[i + j] = 0;
	if (size >= dst_length)
		return (dst_length + ft_strlen(src));
	return (size + ft_strlen(src));
}
///*
//int main(void)
//{
//char dest[10] = "a";
//ft_print_result(ft_strlcat(dest, "lorem ipsum dolor sit amet", 1));
//   write(1, "\n", 1);
// write(1, dest, 15);
//return 0;
//}
//#include <string.h>
//#include <stdio.h>
//int	main(void)
//{
//	char buffer[15] = 'a';
//	char source[] = "Hallo das ist string";
//	ft_strlcat(buffer, source, 1);
//	printf("%s\n", buffer);
//	return (0);
//}
