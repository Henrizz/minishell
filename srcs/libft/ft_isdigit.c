/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 13:00:19 by hzimmerm          #+#    #+#             */
/*   Updated: 2023/11/19 16:31:49 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
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
