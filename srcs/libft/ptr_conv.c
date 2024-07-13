/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ptr_conv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 15:00:30 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/06/10 11:22:51 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_conv_print_ptr(uintptr_t n)
{
	int	length;

	length = 0;
	if (n >= 16)
	{
		length = length + ft_conv_print_ptr(n / 16);
		length = length + ft_conv_print_ptr(n % 16);
	}
	else
	{
		if (n <= 9)
			length = length + ft_printchar(n + '0');
		else
			length = length + ft_printchar(n - 10 + 'a');
	}
	return (length);
}

int	ft_putptr(unsigned long long ptr)
{
	int	length;

	if (!ptr) 
	{
		write(1, "(nil)", 5);
		return (5);
	}
	length = 2;
	ft_printstr("0x");
	if (ptr == 0)
		length = length + ft_printchar('0');
	else
		length = length + ft_conv_print_ptr(ptr);
	return (length);
}
