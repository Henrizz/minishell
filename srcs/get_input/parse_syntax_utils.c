/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_syntax_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Henriette <Henriette@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 13:12:16 by hzimmerm          #+#    #+#             */
/*   Updated: 2024/08/20 17:17:09 by Henriette        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	syntax_check(char *str)
{
	str = NULL;
	(void)str;
	return (0);
	//when symbols are next to | --> syntax error near unexpected token
	//when symbols other than | > < are there when not inside quotes 
	//in case of error return 1
}
