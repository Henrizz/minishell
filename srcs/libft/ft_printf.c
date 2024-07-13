/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 17:25:19 by Henriette         #+#    #+#             */
/*   Updated: 2024/06/10 11:17:49 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// parse string until % is encountered, then check the next index for conv sign
// then each conversion sign gets own situation
// return value is number of characters written

#include "libft.h"

int	ft_conversion(const char c, va_list args)
{
	int	length;

	length = 0;
	if (c == 'c')
		length = ft_printchar(va_arg(args, int));
	else if (c == 's')
		length = ft_printstr(va_arg(args, char *));
	else if (c == 'p')
		length = ft_putptr(va_arg(args, unsigned long long));
	else if (c == 'd' || c == 'i')
		length = ft_printnbr(va_arg(args, int));
	else if (c == 'u')
		length = ft_print_unsint(va_arg(args, unsigned int));
	else if (c == 'x' || c == 'X')
		length = ft_conv_print_hex(va_arg(args, unsigned int), c);
	else if (c == '%')
		length = ft_printchar('%');
	else if (c == '@')
		length = write(1, "%@", 2);
	else
		length = -1;
	return (length);
}

int	ft_printf(const char *format, ...)
{
	int		length;
	int		result;
	int		i;
	va_list	args;

	length = 0;
	i = 0;
	va_start(args, format);
	while (format[i] != 0)
	{
		if (format[i] == '%')
		{
			result = ft_conversion(format[i + 1], args);
			if (result == -1 || format[i + 1] == '\0')
				return (-1);
			length = length + result;
			i++;
		}
		else 
			length = length + ft_printchar(format[i]);
		i++;
	}
	va_end(args);
	return (length);
}

int	ft_conv_print_hex(unsigned int n, const char sign)
{
	int	length;

	length = 0;
	if (n > 15)
	{
		length = length + ft_conv_print_hex(n / 16, sign);
		length = length + ft_conv_print_hex(n % 16, sign);
	}
	else
	{
		if (n <= 9)
			length = length + ft_printchar(n + '0');
		else
		{
			if (sign == 'x')
				length = length + ft_printchar(n - 10 + 'a');
			if (sign == 'X')
				length = length + ft_printchar(n - 10 + 'A');
		}
	}
	return (length);
}
