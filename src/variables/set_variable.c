/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 15:29:51 by orudek            #+#    #+#             */
/*   Updated: 2023/10/06 15:33:20 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_var.h"

static int	replace_variable(t_list *lst, t_var *var, int index)
{
	t_var	*aux;
	char	*new_content;

	if (var->content)
	{
		new_content = ft_strdup(var->content);
		if (!new_content)
			return (0);
	}
	else
		new_content = NULL;
	aux = (t_var *)ft_lstget_index(lst, index)->content;
	if (aux->content)
		free(aux->content);
	aux->content = new_content;
	if (var->type != DEFAULT_VAR)
		aux->type = var->type;
	return (1);
}

int	set_variable(t_list **lst, char *name, char *content, int type)
{
	int		index;
	t_var	*var;
	int		exit_status;

	var = new_var(name, content, type);
	if (!var)
		return (0);
	index = is_in_varlist(*lst, var->name);
	if (index == -1)
	{
		if (type == DEFAULT_VAR)
			var->type = LOCAL_VAR;
		if (!ft_lstadd_back_content(lst, var))
		{
			free_var(var);
			return (0);
		}
		return (1);
	}
	exit_status = replace_variable(*lst, var, index);
	free_var(var);
	return (exit_status);
}
