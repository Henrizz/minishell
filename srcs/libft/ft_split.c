/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 18:20:42 by hzimmerm          #+#    #+#             */
/*   Updated: 2023/11/29 16:01:14 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_words(char const *s, char c)
{
	int	is_word;
	int	i;
	int	count;

	i = 0;
	is_word = 0;
	count = 0;
	while (s[i] != 0)
	{
		if (s[i] != c)
			is_word = 1;
		if (s[i] == c && is_word == 1)
		{
			count++;
			is_word = 0;
		}
		i++;
	}
	if (s[i] == 0 && is_word == 1)
		count++;
	return (count);
}

static char	*ft_strdup_c(const char *s, char c)
{
	char	*dup;
	int		i;
	int		length;

	length = 0;
	i = 0;
	while (s[length] != 0 && s[length] != c)
		length++;
	dup = (char *)malloc(sizeof(char) * (length + 1));
	if (dup == 0)
		return (0);
	while (i < length)
	{
		dup[i] = s[i];
		i++;
	}
	dup[length] = 0;
	return (dup);
}

static void	ft_free_array(char **array, int i)
{
	while (i >= 0)
		free(array[i--]);
	free(array);
}

static void	*ft_loop_outs(char **array, char const *s, char c, int count)
{
	int	j;
	int	i;	

	j = 0;
	i = 0;
	while (i < count)
	{
		while (s[j] == c)
			j++;
		array[i] = ft_strdup_c(&s[j], c);
		if (array[i] == 0)
		{
			ft_free_array(array, i);
			return (0);
		}
		while (s[j] != '\0' && s[j] != c)
			j++;
		i++;
	}
	return ((void *)1);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	int		i;
	int		j;
	int		count;

	i = 0;
	j = 0;
	if (s == 0)
		return (0);
	count = ft_count_words(s, c);
	array = (char **)malloc((count + 1) * sizeof(char *));
	if (!array)
		return (0);
	if (ft_loop_outs(array, s, c, count) == NULL)
		return (0);
	array[count] = 0;
	return (array);
}

// #include <stdio.h>
// int	main(void)
// {
// 	char string[] = "lorem ipsum dolor sit amet, consectetur adipiscing elit";
// 	char c = 't';
// 	char **array;
// 	int	i;
// 	i = 0;
// 	array = ft_split(string, c);
// 	while (array[i] != 0)
// 	{
// 		printf("%s\n", array[i]);
// 		i++;
// 	}
// 	i = 0;
// 	while (array[i] != 0)
// 	{
// 		free(array[i]);
// 		i++;
// 	}
// 	free(array);
// 	return (0);
// }
