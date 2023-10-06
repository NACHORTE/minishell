/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 13:41:05 by orudek            #+#    #+#             */
/*   Updated: 2023/10/06 13:51:11 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

char	*format_args(char *s)
{
	char	*out;
	int		i[2];

	out = malloc(format_arg_len(s) + 1);
	if (!out)
		return (NULL);
	i[0] = 0;
	i[1] = 0;
	s--;
	while (*++s)
	{
		if (i[0] == 0 && (*s == '\'' || *s == '"'))
		{
			i[0] = (*s == '\'') * 2 + (*s == '"');
			continue ;
		}
		else if ((i[0] == 1 && *s == '"') || (i[0] == 2 && *s == '\''))
		{
			i[0] = 0;
			continue ;
		}
		out[i[1]++] = *s;
	}
	out[i[1]] = '\0';
	return (out);
}
