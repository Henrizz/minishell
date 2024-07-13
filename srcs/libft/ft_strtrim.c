/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 15:31:05 by hzimmerm          #+#    #+#             */
/*   Updated: 2023/11/29 16:17:55 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_is_part(char const c, char const *set)
{
	int	i;

	i = 0;
	while (set[i] != 0)
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

static int	ft_find_start(char const *s1, char const *set)
{
	int	i;

	i = 0;
	while (s1[i] != 0)
	{
		if (ft_is_part(s1[i], set) == 1)
			i++;
		else 
			return (i);
	}
	return (-1);
}

static int	ft_find_end(char const *s1, char const *set)
{
	int	i;

	i = ft_strlen(s1) - 1;
	while (i >= 0)
	{
		if (ft_is_part(s1[i], set) == 1)
			i--;
		else 
			return (i);
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*new;
	int		i;
	int		j;
	int		k;

	if (s1 == 0 || set == 0)
		return (0);
	j = ft_find_start(s1, set);
	k = ft_find_end(s1, set);
	i = 0;
	if (k == -1 || j == -1)
	{
		new = malloc(1 * sizeof(char));
		if (new == 0)
			return (0);
		new[i] = 0;
		return (new);
	}
	new = malloc((k - j + 2) * sizeof(char));
	if (new == 0)
		return (0);
	while (j <= k && s1[j] != 0)
		new[i++] = s1[j++];
	new[i] = 0;
	return (new);
}
// #include <stdio.h>
// int	main(void)
// {
// 	char	string[] = "\t   \n\n\n  \n\n\t     \t\t\t\t  ";
// 	char	del[] = " \n\t";
// 	char	*new;
// 	new = ft_strtrim(string, del);
// 	printf("%s\n", new);
// 	free(new);
// 	return (0);
// }
