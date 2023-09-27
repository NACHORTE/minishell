/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_var.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:44:41 by orudek            #+#    #+#             */
/*   Updated: 2023/09/21 16:42:41 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_var.h"

void	free_var(void *var)
{
	if (!var)
		return ;
	if (((t_var *)var)->content)
		free(((t_var *)var)->content);
	if (((t_var *)var)->name)
		free(((t_var *)var)->name);
	free(((t_var *)var));
}
