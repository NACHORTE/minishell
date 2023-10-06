/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 14:28:51 by orudek            #+#    #+#             */
/*   Updated: 2023/10/06 15:31:40 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_string_len(char *s)
{
	int	state;
	int	len;

	state = 0;
	len = 0;
	while (*s)
	{
		if (state == 0 && (*s == '\'' || *s == '"'))
			state = (*s == '\'') * 2 + (*s == '"');
		else if ((state == 1 && *s == '"') || (state == 2 && *s == '\''))
			state = 0;
		if (!state && *s == '|')
			return (len);
		len++;
		s++;
	}
	return (len);
}

static char	*get_string(char **s)
{
	int		len;
	int		i;
	char	*string;

	if (**s == '|')
		(*s)++;
	len = get_string_len(*s);
	string = malloc(len + 1);
	if (string == NULL)
		return (NULL);
	i = -1;
	while (++i < len)
		string[i] = *(*s)++;
	string[i] = 0;
	return (string);
}

static int	get_num_strings(char *s)
{
	int	count;
	int	state;

	count = 1;
	state = 0;
	while (*s)
	{
		if (state == 0 && (*s == '\'' || *s == '"'))
			state = (*s == '\'') * 2 + (*s == '"');
		else if ((state == 1 && *s == '"') || (state == 2 && *s == '\''))
			state = 0;
		if (!state && *s == '|')
			count++;
		s++;
	}
	return (count);
}

char	**split_pipe(char *s)
{
	char	**strings;
	int		num_strings;
	int		i;

	num_strings = get_num_strings(s);
	strings = malloc((sizeof(char *) * (num_strings + 1)));
	if (strings == NULL)
		return (NULL);
	i = 0;
	while (i < num_strings)
	{
		strings[i] = get_string(&s);
		if (!strings[i])
		{
			ft_array_free(strings);
			return (0);
		}
		i++;
	}
	strings[i] = 0;
	return (strings);
}
