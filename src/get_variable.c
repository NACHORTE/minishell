/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 15:22:39 by orudek            #+#    #+#             */
/*   Updated: 2023/09/11 16:43:43 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_var.h"

char	*get_variable(t_list *lst, char *name)
{
	t_var *aux;

	while(lst)
	{
		aux = (t_var *)(lst->content);
		if (!ft_strcmp(aux->name, name))
			return (aux->content); 
		lst = lst->next;
	}
	return (NULL);
}
