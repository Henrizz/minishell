/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 11:52:49 by hzimmerm          #+#    #+#             */
/*   Updated: 2023/12/05 12:48:35 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_find_len(int n)
{
	int	count;

	count = 0;
	if (n == -2147483648)
		return (11);
	if (n <= 0)
	{
		count++;
		n *= -1;
	}
	while (n > 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	int		i;

	i = 0;
	len = ft_find_len(n);
	str = (char *)malloc((len + 1) * sizeof(char));
	if (str == 0)
		return (0);
	if (n == 0)
		str[0] = '0';
	if (n == -2147483648)
		return (ft_memcpy(str, "-2147483648", 12));
	if (n < 0)
	{
		str[0] = '-';
		n *= -1;
	}
	while (n != 0)
	{
		str[len - 1 - i++] = n % 10 + '0';
		n = n / 10;
	}
	str[len] = 0;
	return (str);
}
/*
#include <stdio.h>
int	main(void)
{
	int		arg = -2147483648;
	char	*str;

	str = ft_itoa(arg);
	printf("%s\n", str);
	free(str);
	return (0);
}*/
