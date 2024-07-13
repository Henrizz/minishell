/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Henriette <Henriette@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 23:38:15 by Henriette         #+#    #+#             */
/*   Updated: 2024/03/08 12:16:39 by Henriette        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list *ft_lstmap(t_list *lst, void *(*f)(void *),
void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_node;
	void	*content;

	if (!lst || !del || !f)
		return (NULL);
	new_list = NULL;
	while (lst)
	{
		content = f(lst->content);
		new_node = ft_lstnew(content);
		if (!new_node)
		{
			ft_lstclear(&lst, del);
			return (NULL);
		}
		ft_lstadd_back(&new_list, new_node);
		lst = lst->next;
	}
	return (new_list);
}
/*
#include <stdio.h>
void *multiply_by_two(void *num) {
    int *result = (int *)malloc(sizeof(int));
    if (!result)
        return NULL;
    *result = (*(int *)num) * 2;
    return result;
}

void free_int(void *num) {
    free((int *)num);
}

int main() {
    t_list *lst = NULL;
    int data[] = {1, 2, 3, 4, 5};
    int i = 0;
    while (i < 5)
	{
        int *num = (int *)malloc(sizeof(int));
        *num = data[i];
        ft_lstadd_back(&lst, ft_lstnew(num));
        i++;
    }
    printf("Original list: ");
    t_list *temp = lst;
    while (temp) {
        printf("%d ", *(int *)(temp->content));
        temp = temp->next;
    }
    printf("\n");
    t_list *mapped_list = ft_lstmap(lst, &multiply_by_two, &free_int);
    printf("Mapped list (doubled): ");
    temp = mapped_list;
    while (temp) {
        printf("%d ", *(int *)(temp->content));
        temp = temp->next;
    }
    printf("\n");
    ft_lstclear(&lst, &free_int);
    ft_lstclear(&mapped_list, &free_int);
    return 0;
}*/
