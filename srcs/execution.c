/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Henriette <Henriette@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 10:57:44 by Henriette         #+#    #+#             */
/*   Updated: 2024/08/08 11:09:52 by Henriette        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int execute(t_input **command, t_env *env_list)
{
	(void)env_list;
	set_up_pipes_redirections(command);
	return (0);
}

void set_up_pipes_redirections(t_input **command)
{
	int	pipe_qty;

	pipe_qty = get_cmd_index(command);
	printf("%d\n", pipe_qty);
}

int	get_cmd_index(t_input **command)
{
	t_input *temp;
	int	i;

	temp = *command;
	i = 0;
	while (temp)
	{
		if (temp->next)
			i++;
		temp = temp->next;
	}
	return (i);
}
