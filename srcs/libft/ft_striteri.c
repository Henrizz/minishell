/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 14:44:14 by hzimmerm          #+#    #+#             */
/*   Updated: 2023/11/24 14:56:24 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	int		i;

	i = 0;
	while (s[i] != 0)
	{
		f(i, &s[i]);
		i++;
	}
	s[i] = 0;
}
/*
void	ft_move_one(unsigned int i, char *c)
{
	(void)i;
	*c = *c + 1;
}

#include <stdio.h>
int	main(void)
{
	char	string[] = "abcdef12345";
	char *new;
	new = string;
	ft_striteri(new, *ft_move_one);
	printf("%s\n", new);
	return (0);
}*/
