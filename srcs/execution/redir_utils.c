/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hzimmerm <hzimmerm@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 18:13:42 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/08/08 18:14:53 by hzimmerm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* this function saves the original standard input and standard output in the corresponding int variable before potential redirection */
int	save_in_out(int	*stdin_copy, int *stdout_copy)
{
	*stdin_copy = dup(STDIN_FILENO); // save stdin to reset later
	*stdout_copy = dup(STDOUT_FILENO);
	if (*stdin_copy == -1 || *stdout_copy == -1)
		return (error_return("dup"));
	return (0);
}

/* this function restores the original standard in and out after the command has been executed with their potential redirection */
int	restore_in_out(int	*stdin_copy, int *stdout_copy)
{
	if (dup2(*stdin_copy, STDIN_FILENO) == -1 || dup2(*stdout_copy, STDOUT_FILENO) == -1) //reset to stdin + stdout
	{
		close(*stdin_copy);
		close(*stdout_copy);
		return (error_return("dup2"));
	}
	close(*stdin_copy);
	close(*stdout_copy);
	return (0);
}
