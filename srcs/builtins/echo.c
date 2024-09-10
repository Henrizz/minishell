/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:47:28 by smanriqu          #+#    #+#             */
/*   Updated: 2024/09/09 21:45:13 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	echo(char **str, t_global *global)
{
	int		i;
	size_t	len;

	i = 1;
	if (str[i])
	{
		len = ft_strlen(str[1]);
		if (ft_strncmp(str[1], "-n", 2) == 0)
			i++;
		while (str[i])
		{
			printf("%s", str[i]);
			if (str[i + 1] != NULL)
				printf(" ");
			i++;
		}
		if (ft_strncmp(str[1], "-n", 2) != 0)
			printf("\n");
	}
	global->exit_status = 0;
}
