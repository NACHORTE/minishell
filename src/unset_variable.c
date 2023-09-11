/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_variable.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:09:38 by orudek            #+#    #+#             */
/*   Updated: 2023/09/11 17:15:02 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_var.h"

static int	is_in_list(t_list *lst, char *name)
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

void	unset_variable(t_list **list, char *name)
{
	int index;

	index = is_in_list(*list, name);
	if (index != -1)
		ft_lstpop(list, index, free_var);
}