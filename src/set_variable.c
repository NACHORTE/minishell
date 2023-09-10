/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 15:29:51 by orudek            #+#    #+#             */
/*   Updated: 2023/09/10 17:14:37 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

typedef struct s_variable
{
	char	*name;
	char	*content;
}	t_var;

static int	is_in_list(t_list *lst, char *name)
{
	t_var var;
	int	index;

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

static t_list	*ft_lstget_index(t_list *lst, int index)
{
	while (lst && index-- > 0)
		lst = lst->next;
	return (lst);
}

char	set_variable(t_list **lst, t_var *var)
{
	int	index;
	t_list	*new_lst;
	t_list	*aux;

	if (!var)
		return (0);
	index = is_in_list(*lst, var->name); 
	if (index == -1)
		ft_lstadd_back_content(lst, var);
	else
	{
		new_lst = ft_lstnew(var);
		if (!new_lst)
			return (0);
		if (index == 0)
		{
			new_lst->next = (*lst)->next;
			free ((*lst)->content);
			free (*lst);
			*lst = new_lst;
		}
		else
		{
			aux = ft_lstget_index(*lst, index - 1);
			new_lst->next = aux->next->next;
			free (aux->next->content);
			free (aux->next);
			aux->next = new_lst;
		}
	}
	return (1);
}

void print_list(t_list *list)
{
	int i = 0;

	if (!list)
		printf("list[0]: NULL\n");
	while(list)
	{
		printf("list[%d]: (%p)\n",i,list);
		printf("\tvars.name:\t[%s]\n\tvars.content:\t[%s]\n",((t_var *)list->content)->name, ((t_var *)list->content)->content);	
		list = list->next;
		i++;
	}
}

void	ft_free_var(t_var *var)
{
	free(var->content);
	free(var->name);
	free(var);
}

void	ft_free_list(t_list *list)
{
	t_list aux;

	while (list)
	{
		aux.next = list->next;
		ft_free_var(list->content);
		free(list);
		list = aux.next;
	}
}
/*
int main()
{
	t_var **vars = malloc(sizeof(t_var *) * 5);
	t_list	*list = NULL;

	vars[0] = malloc (sizeof(t_var));
	vars[1] = malloc (sizeof(t_var));
	vars[2] = malloc (sizeof(t_var));
	vars[3] = malloc (sizeof(t_var));
	vars[4] = malloc (sizeof(t_var));
	vars[0]->name = "name0";
	vars[0]->content = "content0";
	vars[1]->name = "name0";
	vars[1]->content = "content1";
	vars[2]->name = "name2";
	vars[2]->content = "content2";
	vars[3]->name = "name3";
	vars[3]->content = "content3";
	vars[4]->name = "name2";
	vars[4]->content = "content4";
	
	set_variable(&list, vars[0]);
	set_variable(&list, vars[1]);
	set_variable(&list, vars[2]);
	set_variable(&list, vars[3]);
	set_variable(&list, vars[4]);
	print_list(list);
	ft_free_list(list);
	free(vars);
	return (1);
}*/
