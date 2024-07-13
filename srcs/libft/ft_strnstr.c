/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 16:20:02 by Henriette         #+#    #+#             */
/*   Updated: 2023/11/29 16:13:30 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	nee_len;
	char	*crash;

	i = 0;
	crash = NULL;
	if (haystack == NULL)
		*crash = 0;
	nee_len = ft_strlen(needle);
	if (nee_len == 0)
		return ((char *)haystack);
	while (i + nee_len <= len && haystack[i] != 0)
	{
		if (ft_strncmp(haystack + i, needle, nee_len) == 0)
			return ((char *)(haystack + i));
		i++;
	}
	return (0);
}
/*
#include <stdio.h>
#include <string.h>
int	main(void)
{
	const char *largestring = "Nadst die Nadel im Heuhaufen";
    const char *smallstring = "Nadel";
    char *ptr1;
	//char *ptr2;
	ptr1 = ft_strnstr(largestring, smallstring, 20);
	printf("%s\n", ptr1);
    //ptr2 = strnstr(NULL, "fake", 3);
	//printf("%s\n", ptr2);
	return (0);
}*/
