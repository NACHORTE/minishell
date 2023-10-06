/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 13:43:57 by orudek            #+#    #+#             */
/*   Updated: 2023/10/06 13:51:22 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

static int	redir_len(char *s)
{
	int	len;

	len = 1;
	if (s[0] == s[1])
		len++;
	if (s[1] == '<' || s[1] == '>')
		s++;
	s++;
	while (*s == ' ')
		s++;
	len += format_arg_len(s);
	return (len);
}

static void	cpy_redir(char *out, char *s)
{
	int	state;

	*out++ = *s++;
	if (s[-1] == s[0])
		*out++ = *s++;
	while (*s == ' ' || *s == '<' || *s == '>')
		s++;
	state = 0;
	s--;
	while (*++s)
	{
		if (state == 0 && (*s == '\'' || *s == '"'))
		{
			state = (*s == '\'') * 2 + (*s == '"');
			continue ;
		}
		else if ((state == 1 && *s == '"') || (state == 2 && *s == '\''))
		{
			state = 0;
			continue ;
		}
		*out++ = *s;
	}
	*out = '\0';
}

char	*format_redir(char *s)
{
	char	*out;

	out = malloc(redir_len(s) + 1);
	if (!out)
		return (NULL);
	cpy_redir(out, s);
	if (((out[1] == '<' || out[1] == '>') && !out[2]) || !out[1])
	{
		free(out);
		return ((void *)return_msg("syntax error near unexpected token",
				2, 0));
	}
	return (out);
}
