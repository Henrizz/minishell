/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 13:00:09 by hzimmerm          #+#    #+#             */
/*   Updated: 2023/11/19 16:31:32 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	else
		return (0);
}
/*
#include <stdio.h>
int	main(void)
{
	int c;
	
	c = 'd';
	if (ft_isalpha(c) == 1)
		printf("yes\n");
	else 
		printf("no\n");
	return (0);
}*/
