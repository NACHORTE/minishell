/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   varlist_to_array.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 22:46:57 by oscar             #+#    #+#             */
/*   Updated: 2023/10/06 15:33:39 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_var.h"

static int	var_to_str(t_var *var, char **str, int type)
{
	char	*out;
	int		len[2];

	if (!var || (type != DEFAULT_VAR && type != var->type) || !var->content)
		return (0);
	len[0] = ft_strlen(var->name);
	len[1] = ft_strlen(var->content);
	out = malloc(sizeof(char) * (len[0] + len[1] + 2));
	if (!out)
		return (-1);
	ft_strlcpy(out, var->name, len[0] + 1);
	ft_strlcpy(out + len[0], "=", 2);
	ft_strlcpy(out + len[0] + 1, var->content, len[1] + 1);
	*str = out;
	return (1);
}

int	varlist_len(t_list *varlist, int type)
{
	t_var	*var;
	int		count;

	count = 0;
	while (varlist)
	{
		var = (t_var *)varlist->content;
		if (var && (type == DEFAULT_VAR || type == var->type) && var->content)
			count++;
		varlist = varlist->next;
	}
	return (count);
}

static char	**save_vars(char **out, t_list *varlist, int len, int type)
{
	int	i;
	int	ret;

	i = 0;
	while (i < len)
	{
		ret = var_to_str((t_var *)varlist->content, &out[i], type);
		if (ret == 1)
			i++;
		else if (ret == -1)
		{
			out[i] = NULL;
			ft_array_free(out);
			return (NULL);
		}
		varlist = varlist->next;
	}
	out[i] = 0;
	return (out);
}

char	**varlist_to_array(t_list *varlist, int type)
{
	char	**out;
	int		len;

	if (!varlist)
		return (NULL);
	len = varlist_len(varlist, type);
	out = malloc(sizeof(char *) * (len + 1));
	if (!out)
		return (NULL);
	return (save_vars(out, varlist, len, type));
}
