/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 14:36:51 by orudek            #+#    #+#             */
/*   Updated: 2023/09/29 14:37:54 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

static int	save_cmds(char **input, char **parsed)
{
	int	i;
	int	args;

	i = 0;
	args = 0;
	while (input[i])
	{
		if (input[i][0] != '<' && input[i][0] != '>')
		{
			parsed[args] = ft_strdup(input[i]);
			if (!parsed[args])
			{
				printf("problema dup\n");
				ft_array_free(parsed);
				return (1);
			}
			args++;
		}
		i++;
	}
	parsed[args] = 0;
	return (0);
}

char	**parse_cmd(char **input)
{
	int		i;
	char	**parsed;
	int		args;

	i = 0;
	args = 0;
	while (input[i])
	{
		if (input[i][0] != '<' && input[i][0] != '>')
			args++;
		i++;
	}
	parsed = malloc(sizeof(char *) * (args + 1));
	if (!parsed)
		return (NULL);
	if (save_cmds(input, parsed))
		return (NULL);
	return (parsed);
}
