/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_to_varlist.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 22:44:56 by oscar             #+#    #+#             */
/*   Updated: 2023/10/06 15:32:43 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_var.h"

static int	str_find_char(char *str, char c)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] == c)
			return (i);
	return (i);
}

static char	split_var(t_var *var, char *str)
{
	int	pos;
	int	len;

	pos = str_find_char(str, '=');
	len = (int) ft_strlen(str);
	var->name = malloc (pos + 1);
	if (pos < len)
		var->content = malloc (len - pos);
	else
		var->content = NULL;
	if (!var->name || (len != pos && !var->content))
	{
		if (var->name)
			free(var->name);
		if (var->content)
			free(var->content);
		return (0);
	}
	ft_strlcpy(var->name, str, pos + 1);
	if (var->content)
		ft_strlcpy(var->content, ft_strrchr(str, '=') + 1, len - pos);
	return (1);
}

t_list	*array_to_varlist(char **array)
{
	t_var	var;
	t_list	*out;
	int		i;

	if (!array)
		return (NULL);
	out = NULL;
	i = -1;
	while (array[++i])
	{
		if (!split_var(&var, array[i]))
			continue ;
		if (!set_variable(&out, var.name, var.content, ENV_VAR))
		{
			ft_lstfree(out, free_var);
			free(var.name);
			free(var.content);
			return (NULL);
		}
		free(var.name);
		free(var.content);
	}
	return (out);
}
