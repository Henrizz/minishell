/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Henriette <Henriette@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 15:13:31 by Henriette         #+#    #+#             */
/*   Updated: 2024/07/23 16:38:37 by Henriette        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* just an exit function to save space in main functions. Possibly needs to be later completed with signal handlings? or also free functions */
int	exit_shell(char *message, int exit_status)
{
		if (exit_status == EXIT_SUCCESS && message != NULL)
			ft_putstr_fd(message, 1);
		if (exit_status == EXIT_FAILURE && message != NULL)
			perror(message);
		exit(exit_status);
}

void	free_command(t_input **command)
{
	t_input *temp;

	temp = *command;
	while (*command != NULL)
	{
		temp = *command;
		*command = (*command)->next;
		free_array(temp->words);
		free_array(temp->red_in);
		free_array(temp->red_out);
		free_array(temp->heredoc);
		free_array(temp->app_out);
		free(temp);
	}
	*command = NULL;
}

/* to free a twodimensional array of strings */
void	free_array(char **str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return ;
	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str);
}
