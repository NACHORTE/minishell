/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_in_varlist.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 14:19:47 by orudek            #+#    #+#             */
/*   Updated: 2023/10/06 15:33:03 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_var.h"

int	is_in_varlist(t_list *lst, char *name)
{
	t_var	var;
	int		index;

	index = 0;
	while (lst)
	{
		var = *(t_var *)lst->content;
		if (!ft_strcmp(var.name, name))
			return (index);
		lst = lst->next;
		index++;
	}
	return (-1);
}
