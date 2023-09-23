/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 15:22:39 by orudek            #+#    #+#             */
/*   Updated: 2023/09/21 21:14:09 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_var.h"

/*	[x] Differentiate between name exists and content is null \
		and name doesn't exist
		get_variable(nacho) "doesnt exist here"
			should return NULL
		export nacho
			env: nacho=NULL
		env | grep nacho
			Nothing shows but nacho is still in env list exported
		get_variable(nacho) "nacho is in env so it should show something"
		Maybe return returns 1 if exists and 0 if not, and another parameter
		could be the return for content
		char	get_variable(t_list *lst, char *name, char **content)
*/
int	get_variable(t_list *lst, char *name, char **content)
{
	t_var *aux;

	while(lst)
	{
		aux = (t_var *)(lst->content);
		if (!ft_strcmp(aux->name, name))
		{
			*content = aux->content;
			return (1);
		}
		lst = lst->next;
	}
	return (0);
}
