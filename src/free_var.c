/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_var.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:44:41 by orudek            #+#    #+#             */
/*   Updated: 2023/09/11 16:02:07 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_var.h"

void	free_var(void *var)
{
	free(((t_var *)var)->content);
	free(((t_var *)var)->name);
	free(((t_var *)var));
}