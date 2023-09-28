/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 15:45:28 by orudek            #+#    #+#             */
/*   Updated: 2023/09/28 15:50:17 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_var.h"

t_var	*new_var(char *name, char *content, int type)
{
	t_var	*var;

	if (!name || !*name)
		return (NULL);
	var = malloc(sizeof(t_var));
	if (!var)
		return (NULL);
	var->name = ft_strdup(name);
	if (content)
		var->content = ft_strdup(content);
	else
		var->content = NULL;
	if (!var->name || (content && !var->content))
	{
		free_var(var);
		return (NULL);
	}
	var->type = type;
	return (var);
}