/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 15:29:51 by orudek            #+#    #+#             */
/*   Updated: 2023/09/11 16:43:03 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_var.h"

static int	is_in_list(t_list *lst, char *name)
{
	t_var	var;
	int		index;

	index = 0;
	while (lst)
	{
		var = *(t_var *)lst->content;
		if (!ft_strcmp(var.name, name))
			return (index);
		lst = lst->next;
		index++;
	}
	return (-1);
}

static char	replace_variable(t_list **lst, t_var *var, int index)
{
	t_list	*aux;
	t_list	*aux2;
	t_list	*new_lst;

	new_lst = ft_lstnew(var);
	if (!new_lst)
		return (0);
	if (index == 0)
	{
		ft_lstpop(lst, 0, free_var);
		ft_lstadd_front(lst, new_lst);
	}
	else
	{
		aux = ft_lstget_index(*lst, index - 1);
		aux2 = aux->next;
		aux->next = new_lst;
		new_lst->next = aux2->next;
		ft_lstdelone(aux2, free_var);
	}
	return (1);
}

char	set_variable(t_list **lst, t_var *var)
{
	int		index;

	if (!var)
		return (0);
	index = is_in_list(*lst, var->name);
	if (index == -1)
		return (ft_lstadd_back_content(lst, var));
	return (replace_variable(lst, var, index));
}

/*COMENTAR A PARTIR DE AQUI*//*
void print_list(t_list *list)
{
	int i = 0;//COMENTAR ESTA FUNCION SI NO SE USA MAIN

	if (!list)
		printf("list[0]: NULL\n");
	while(list)
	{
		printf("list[%d]: (%p) (%p)\n",i,list, list->next);
		printf("\tvars.name:\t[%s]\n\tvars.content:\t[%s]\n",
		((t_var *)list->content)->name, ((t_var *)list->content)->content);
		list = list->next;
		i++;
	}
	printf("______\n");
}
int main()
{
	t_var **vars = malloc(sizeof(t_var *) * 5);
	t_list	*list = NULL;

	vars[0] = malloc (sizeof(t_var));
	vars[1] = malloc (sizeof(t_var));
	vars[2] = malloc (sizeof(t_var));
	vars[3] = malloc (sizeof(t_var));
	vars[4] = malloc (sizeof(t_var));
	vars[0]->name = ft_strdup("name0");
	vars[0]->content = ft_strdup("content0");
	vars[1]->name = ft_strdup("name0");
	vars[1]->content = ft_strdup("content1");
	vars[2]->name = ft_strdup("name2");
	vars[2]->content = ft_strdup("content2");
	vars[3]->name = ft_strdup("name3");
	vars[3]->content = ft_strdup("content3");
	vars[4]->name = ft_strdup("name2");
	vars[4]->content = ft_strdup("content4");
	
	set_variable(&list, vars[0]);
	set_variable(&list, vars[1]);
	set_variable(&list, vars[2]);
	set_variable(&list, vars[3]);
	set_variable(&list, vars[4]);
	print_list(list);
	ft_lstfree(&list, free_var);
	free(vars);
	return (0);
}*/
