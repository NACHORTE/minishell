/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 22:37:17 by orudek            #+#    #+#             */
/*   Updated: 2023/10/06 13:27:31 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "t_var.h"

static t_list	*expand_str(char *name, t_list *varlist,
	t_list *(*str_to_lst)(char *))
{
	char	*expanded_str;
	t_list	*out;

	expanded_str = get_expanded_str(name, varlist);
	if (!expanded_str)
		return (NULL);
	out = str_to_lst(expanded_str);
	free(expanded_str);
	if (!out)
		return (NULL);
	return (out);
}

static int	expand_list(t_list **lst, t_list *varlist,
	t_list *(*str_to_lst)(char *))
{
	t_list	*last;
	t_list	*new_lst;
	t_list	*aux;
	t_list	*new_lst_last;

	last = NULL;
	aux = *lst;
	while (aux)
	{
		new_lst = expand_str((char *)aux->content, varlist, str_to_lst);
		if (!new_lst)
			return (0);
		new_lst_last = ft_lstlast(new_lst);
		new_lst_last->next = aux->next;
		if (last)
			last->next = new_lst;
		else
			*lst = new_lst;
		ft_lstdelone(aux, free);
		last = new_lst_last;
		aux = new_lst_last->next;
	}
	return (1);
}

int	expand_variables(t_list *cmd, t_list *varlist)
{
	t_arg_redir	*aux;

	while (cmd)
	{
		aux = (t_arg_redir *)cmd->content;
		if (!expand_list(&aux->args, varlist, str_to_arg)
			|| !expand_list(&aux->redir, varlist, str_to_redir))
			return (0);
		cmd = cmd->next;
	}
	return (1);
}
