/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_to_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 13:25:16 by orudek            #+#    #+#             */
/*   Updated: 2023/10/06 13:27:10 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "error.h"

static int	arg_len(char *s)
{
	int	i;
	int	state;

	i = -1;
	state = 0;
	while (s[++i]
		&& (state || (s[i] != ' ' && s[i] != '<' && s[i] != '>')))
	{
		if (state == 0 && (s[i] == '\'' || s[i] == '"'))
			state = (s[i] == '\'') * 2 + (s[i] == '"');
		else if ((state == 1 && s[i] == '"') || (state == 2 && s[i] == '\''))
			state = 0;
	}
	return (i);
}

static char	*get_arg(char **cmd)
{
	int		len;
	char	*out;

	len = arg_len(*cmd);
	out = malloc(sizeof(char) * (len + 1));
	if (!out)
		return (NULL);
	ft_strlcpy(out, *cmd, len + 1);
	(*cmd) += len;
	return (out);
}

t_list	*str_to_arg(char *str)
{
	t_list	*out;
	char	*arg;

	out = NULL;
	while (1)
	{
		while (*str == ' ')
			str++;
		arg = get_arg(&str);
		if (!arg || !ft_lstadd_back_content(&out, arg))
		{
			ft_lstfree(out, free);
			if (arg)
				free(arg);
			return ((void *)return_msg("Couldn't allocate memory", 2, 0));
		}
		if (!*str)
			break ;
	}
	return (out);
}
