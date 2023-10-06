/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 15:22:39 by orudek            #+#    #+#             */
/*   Updated: 2023/10/06 15:32:21 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_var.h"

int	get_variable(t_list *lst, char *name, t_var **var)
{
	t_var	*aux;

	if (!name || !*name)
		return (0);
	while (lst)
	{
		aux = (t_var *)(lst->content);
		if (!ft_strcmp(aux->name, name))
		{
			if (var)
			{
				aux = dup_var(aux);
				if (!aux)
					return (-1);
				*var = aux;
			}
			return (1);
		}
		lst = lst->next;
	}
	return (0);
}
