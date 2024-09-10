/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:47:28 by smanriqu          #+#    #+#             */
/*   Updated: 2024/09/10 13:00:53 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	echo(char **str, t_global *global)
{
	int		i;

	i = 1;
	if (str[i])
	{
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
