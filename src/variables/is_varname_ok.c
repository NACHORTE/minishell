/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_varname_ok.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 18:28:01 by orudek            #+#    #+#             */
/*   Updated: 2023/09/24 19:04:55 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_allowed_char(char c)
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

/*int main(int c, char **v)
{
	if (c != 2)
		return 1;
	if (is_varname_ok(v[1]))
		printf("VAR OK\n");
	else
		printf("BAD VAR\n");
	return (0);
}*/