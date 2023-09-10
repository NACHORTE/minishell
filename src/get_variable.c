/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 15:22:39 by orudek            #+#    #+#             */
/*   Updated: 2023/09/10 17:21:06 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

typedef struct s_variable
{
	char	*name;
	char	*content;
}	t_var;

t_var	*get_variable(t_list *list, char *name)
{
	t_var *aux;

	while(list)
	{
		aux = (t_var *)list->content;
		if (!strcmp(aux, name))
			return (aux); 
		list = list->next;
	}
	return (NULL);
}