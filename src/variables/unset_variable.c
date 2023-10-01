/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_variable.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:09:38 by orudek            #+#    #+#             */
/*   Updated: 2023/10/01 16:46:01 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_var.h"

void	unset_variable(t_list **list, char *name)
{
	int	index;

	index = is_in_varlist(*list, name);
	if (index != -1)
		ft_lstpop(list, index, free_var);
}
