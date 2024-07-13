/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:25:11 by Henriette         #+#    #+#             */
/*   Updated: 2023/11/24 17:34:12 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	size_t	i;
	void	*ptr;

	i = 0;
	ptr = malloc(count * size);
	if (ptr == 0)
		return (0);
	while (i < count * size)
	{
		((char *)ptr)[i] = 0;
		i++;
	}
	return (ptr);
}
/*
int main(void)
{
    char *str = (char *)ft_calloc(30, 1);

    if (!str)
    {
        write(1, "NULL", 4);
    }
    else
    {
        write(1, str, 30);
        free(str);
    }

    return 0;
}*/
