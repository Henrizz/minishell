/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Henriette <Henriette@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 15:13:31 by Henriette         #+#    #+#             */
/*   Updated: 2024/07/13 15:33:40 by Henriette        ###   ########.fr       */
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
