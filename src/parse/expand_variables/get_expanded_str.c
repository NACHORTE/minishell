/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_expanded_str.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 13:11:48 by orudek            #+#    #+#             */
/*   Updated: 2023/10/06 13:27:49 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_var.h"

static char	find_var(char **var, char *str, int len, t_list *list)
{
	int	i;

	str++;
	while (list)
	{
		i = -1;
		while (++i < len)
		{
			if (str[i] != ((t_var *)list->content)->name[i])
				break ;
			if (i == len - 1 && ((t_var *)list->content)->name[i + 1] == '\0')
			{
				*var = ((t_var *)list->content)->content;
				return (1);
			}
		}
		list = list->next;
	}
	return (0);
}

static int	var_len(char *s, int *len, t_list *varlist)
{
	int		i;
	char	*var;

	i = 0;
	while (s[i] && !is_end_of_var(s[i]))
		i++;
	if (!find_var(&var, s, i - 1, varlist))
		return (i);
	*len += ft_strlen(var);
	return (i);
}

static int	expanded_len(char *str, t_list *varlist)
{
	int	len;
	int	state;

	len = 0;
	state = 0;
	while (*str)
	{
		if (state == 0 && (*str == '\'' || *str == '"'))
			state = (*str == '\'') * 2 + (*str == '"');
		else if ((state == 1 && *str == '"') || (state == 2 && *str == '\''))
			state = 0;
		if (state != 2 && *str == '$' && !is_end_of_var(*(str + 1)))
			str += var_len(str, &len, varlist);
		else
		{
			len++;
			str++;
		}
	}
	return (len);
}

static int	cat_arg(char *out, char **s, t_list *varlist)
{
	int		len;
	int		j;
	int		i;
	char	*var;

	len = 0;
	i = var_len(*s, &len, varlist);
	find_var(&var, *s, i - 1, varlist);
	j = -1;
	while (++j < len)
		*out++ = *var++;
	(*s) += i;
	return (len);
}

char	*get_expanded_str(char *str, t_list *varlist)
{
	char	*out;
	int		len;
	int		i;
	int		state;

	len = expanded_len(str, varlist);
	out = malloc(len + 1);
	if (!out)
		return (NULL);
	i = 0;
	state = 0;
	while (i < len)
	{
		if (state == 0 && (*str == '\'' || *str == '"'))
			state = (*str == '\'') * 2 + (*str == '"');
		else if ((state == 1 && *str == '"') || (state == 2 && *str == '\''))
			state = 0;
		if (state != 2 && *str == '$' && !is_end_of_var(*(str + 1)))
			i += cat_arg(&out[i], &str, varlist);
		else
			out[i++] = *str++;
	}
	out[i] = 0;
	return (out);
}
