/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 14:19:57 by hzimmerm          #+#    #+#             */
/*   Updated: 2023/11/24 14:42:22 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	char	*new;
	int		len;

	i = 0;
	len = ft_strlen(s);
	new = (char *)malloc((len + 1) * sizeof(char));
	if (new == 0)
		return (0);
	while (s[i] != 0)
	{
		new[i] = f(i, s[i]);
		i++;
	}
	new[i] = 0;
	return (new);
}
/*
char	ft_move_one(unsigned int i, char c)
{
	(void)i;
	c = c + 1;
	return (c);
}

#include <stdio.h>
int	main(void)
{
	char	string[] = "";
	char	*new;
	int	i;
	i = 0;
	new = ft_strmapi(string, *ft_move_one);
	printf("%s\n", new);
	free(new);
	return (0);
}*/
