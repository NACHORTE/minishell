/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_to_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 13:23:04 by orudek            #+#    #+#             */
/*   Updated: 2023/10/06 13:26:23 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "error.h"

static int	redir_format_ok(char *s)
{
	int	state;

	state = 0;
	if (*s != '<' && *s != '>')
		return (0);
	s++;
	if (*s == '<' || *s == '>')
		s++;
	while (*s == ' ')
		s++;
	while (*s)
	{
		if (state == 0 && (*s == '\'' || *s == '"'))
			state = (*s == '\'') * 2 + (*s == '"');
		else if ((state == 1 && *s == '"') || (state == 2 && *s == '\''))
			state = 0;
		if (state == 0 && *s == ' ')
			return (return_msg("Ambiguous redirect", 2, 0));
		s++;
	}
	return (1);
}

t_list	*str_to_redir(char *str)
{
	t_list	*out;
	char	*new_str;

	if (!redir_format_ok(str))
		return (NULL);
	new_str = ft_strdup(str);
	if (!new_str)
		return (NULL);
	out = NULL;
	if (!ft_lstadd_back_content(&out, new_str))
		free(new_str);
	return (out);
}
