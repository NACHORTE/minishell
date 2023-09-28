/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 15:43:45 by orudek            #+#    #+#             */
/*   Updated: 2023/09/28 15:45:16 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_var.h"

t_var	*dup_var(t_var *var)
{
	return (new_var(var->name, var->content, var->type));
}