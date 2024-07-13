/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Henriette <Henriette@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 19:20:50 by Henriette         #+#    #+#             */
/*   Updated: 2024/03/08 12:10:00 by Henriette        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*temp;
	
	if (!*lst)
		*lst = new;
	else
	{
		temp = *lst;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
}
/*
#include <stdio.h>
int main() 
{
    t_list *lst = NULL;
	t_list	*new_node;
	t_list	*start = NULL;
	int	i;
    int	data[] = {1, 2, 3};
	int	*num;

	i = 0;
    while (i < 3)
	{
        num = (int *)malloc(sizeof(int));
        *num = data[i];
        new_node = (t_list *)malloc(sizeof(t_list));
        new_node->content = num;
        new_node->next = NULL;
        ft_lstadd_back(&lst, new_node);
		if (!start)
			start = lst;
		i++;
    }
    printf("Original list: ");
    while (start)
	{
        printf("%d ", *(int *)start->content);
        start = start->next;
    }
    printf("\n");
	start = lst;
    num = (int *)malloc(sizeof(int));
    *num = 4;
    new_node = (t_list *)malloc(sizeof(t_list));
    new_node->content = num;
    new_node->next = NULL;
    ft_lstadd_back(&lst, new_node);
    printf("List after adding 4 to the back: ");
     while (start)
	{
        printf("%d ", *(int *)start->content);
        start = start->next;
    }
    printf("\n");
    t_list *temp;
    while (lst) {
        temp = lst;
        lst = lst->next;
        free(temp->content);
        free(temp);
    }

    return 0;
}*/
