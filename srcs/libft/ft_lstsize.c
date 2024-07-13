/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Henriette <Henriette@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 18:52:20 by Henriette         #+#    #+#             */
/*   Updated: 2024/03/08 12:22:13 by Henriette        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_lstsize(t_list *lst)
{
	int	i;
	t_list	*temp;
	
	i = 0;
	temp = lst;
	while (temp)
	{
		i++;
		temp = temp->next;
	}
	return (i);
}
/*
#include <stdio.h>
int	main(void)
{
t_list *head = NULL;
    t_list *second = NULL;
    t_list *third = NULL;

    head = (t_list*)malloc(sizeof(t_list));
    second = (t_list*)malloc(sizeof(t_list));
    third = (t_list*)malloc(sizeof(t_list));

    head->content = "Hello";
    head->next = second;
    second->content = "World";
    second->next = third;
    third->content = "!";
    third->next = NULL;
	
	printf("Size of the linked list: %d\n", ft_lstsize(head));
	free(head);
    free(second);
    free(third);

    return 0;
}*/
