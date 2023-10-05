/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_varname_ok.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 18:28:01 by orudek            #+#    #+#             */
/*   Updated: 2023/10/05 14:37:30 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_var.h"

static int	is_allowed_char(char c)
{
	if (c == '_'
		|| ft_isdigit(c)
		|| ft_isalpha(c))
		return (1);
	return (0);
}

int	is_varname_ok(const char *name)
{
	if (!name)
		return (0);
	if (!ft_isalpha(*name) && *name != '_')
		return (0);
	name++;
	while (*name)
	{
		if (!is_allowed_char(*name))
			return (0);
		name++;
	}
	return (1);
}
