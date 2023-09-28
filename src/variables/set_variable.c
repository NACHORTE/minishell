/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/10 15:29:51 by orudek            #+#    #+#             */
/*   Updated: 2023/09/28 17:31:05 by orudek           ###   ########.fr       */
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

/*COMENTAR A PARTIR DE AQUI*/
/*
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
	vars[0]->content = NULL;//ft_strdup("content0");
	vars[1]->name = ft_strdup("name0");
	vars[1]->content = ft_strdup("content1");
	vars[2]->name = ft_strdup("name2");
	vars[2]->content = ft_strdup("content2");
	vars[3]->name = ft_strdup("name3");
	vars[3]->content = ft_strdup("content3");
	vars[4]->name = ft_strdup("name4");
	vars[4]->content = ft_strdup("content4");
	
	set_variable(&list, vars[0]->name, vars[0]->content);
	//set_variable(&list, vars[1]->name, vars[1]->content);
	set_variable(&list, vars[2]->name, vars[2]->content);
	set_variable(&list, vars[3]->name, vars[3]->content);
	set_variable(&list, vars[4]->name, vars[4]->content);
	print_list(list);
	set_variable(&list, vars[1]->name, vars[1]->content);
	unset_variable(&list, "name2");
	//unset_variable(&list, "name0");
	unset_variable(&list, "name4");
	print_list(list);
	ft_lstfree(&list, free_var);
	for (int i = 0; i < 5; i++)
		free(vars[i]);
	free(vars);
	return (1);
}*/
