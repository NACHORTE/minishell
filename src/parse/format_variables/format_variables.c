/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 22:37:17 by orudek            #+#    #+#             */
/*   Updated: 2023/10/06 13:44:47 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "t_var.h"

void	pop_arg(t_list **lst, t_list **aux)
{
	t_list	*previous_lst;

	if (*aux == *lst)
	{
		*lst = (*lst)->next;
		ft_lstdelone(*aux, free);
		*aux = *lst;
		return ;
	}
	previous_lst = *lst;
	while (previous_lst->next != *aux)
		previous_lst = previous_lst->next;
	previous_lst->next = (*aux)->next;
	ft_lstdelone(*aux, free);
	*aux = previous_lst->next;
}

static int	format_list(t_list **lst, char *(*format_str)(char *))
{
	char	*formatted_str;
	t_list	*aux;

	aux = *lst;
	while (aux)
	{
		if (!*(char *)aux->content)
		{
			pop_arg(lst, &aux);
			continue ;
		}
		formatted_str = format_str((char *)aux->content);
		if (!formatted_str)
			return (0);
		free(aux->content);
		aux->content = formatted_str;
		aux = aux->next;
	}
	return (1);
}

int	format_variables(t_list *cmd)
{
	t_arg_redir	*aux;

	while (cmd)
	{
		aux = (t_arg_redir *)cmd->content;
		if (!format_list(&aux->args, format_args)
			|| !format_list(&aux->redir, format_redir))
			return (0);
		cmd = cmd->next;
	}
	return (1);
}
