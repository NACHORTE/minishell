/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_variable.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:09:38 by orudek            #+#    #+#             */
/*   Updated: 2023/09/28 15:53:45 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_var.h"

void	unset_variable(t_list **list, char *name)
{
	int index;

	index = is_in_varlist(*list, name);
	if (index != -1)
		ft_lstpop(list, index, free_var);
}