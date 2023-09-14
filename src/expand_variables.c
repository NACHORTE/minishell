/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 22:37:17 by orudek            #+#    #+#             */
/*   Updated: 2023/09/14 19:44:00 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "t_var.h"

static void	update_state(int *state, char c)
{
	if (*state == 0 && (c == '\'' || c == '"'))
		*state = (c == '\'') * 2 + (c == '"');
	else if ((*state == 1 && c == '"') || (*state == 2 && c == '\''))
		*state = 0;
}

static char *find_var(char *str, int len, t_list *list)
{
	if (*str == '$')
		str++;
	while (list)
	{
		if (!ft_strncmp(str,((t_var *)list->content)->name, len))
			return (((t_var *)list->content)->content);
		list = list->next;
	}
	return (NULL);
}

static int	arg_len(char *str, int *len, t_list *local, t_list *env)
{
	int	i;
	char	*var;

	i = 1; //the first character of the string is a $ so skip it
	while (str[i])
	{
		if (str[i] == ' ' || str[i] == '"' || str[i] == '\'' || str[i] == '$')
			break ;
		i++; // can be optimized initializing i to 0, and doiing ++i on the loop condition
	}
	if (i == 1)
		return (i);
	var = find_var(str, i - 1, local);
	if (!var)
		var = find_var(str, i - 1, env);
	if (!var)
		return (i);
	*len += ft_strlen(var);
	return (i);
}

static int	expanded_len(char *str, t_list *local, t_list *env)
{
	int	len;
	int	state;

	len = 0;
	state = 0;
	while (*str)
	{
		update_state(&state, *str);
		if (state != 2 && *str == '$')
			str += arg_len(str, &len, local, env);
		else
		{
			len++;
			str++;
		}
	}
	return (len);
}

static int	dup_arg(char *out, char **str, t_list *local, t_list *env)
{
	int	len;
	int j;
	int	i;
	char *var;

	len = 0;
	i = arg_len(*str, &len, local, env);
	if (len == 0)
	{
		(*str) += i;
		return (0);
	}
	var = find_var(*str, i - 1, local);
	if (!var)
		var = find_var(*str, i - 1, env);
	if (!var)
		return (i);
	j = -1;
	while (++j < len)
		*out++ = *var++;
	(*str) += i;
	return (len);
}

char	*expand_variables(char *str, t_list *local, t_list *env)
{
	char	*out;
	int		len;
	int		i;
	int		state;

	len = expanded_len(str ,local, env);
	out = malloc(len + 1);
	if (!out)
		return (NULL);
	i = 0;
	state = 0;
	while(i < len)
	{
		update_state(&state, *str);
		if (state != 2 && *str == '$')
			i += dup_arg(&out[i], &str, local, env);
		else
			out[i++] = *str++;
	}
	out[i] = 0;
	return out;
}


/*COMENTAR A PARTIR DE AQUI*//*
#include "t_var.h"

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

int main(int c, char **v)
{
	t_var **vars = malloc(sizeof(t_var *) * 5);
	t_list	*local = NULL;
	t_list	*env = NULL;

	if (c == 1)
		return 1;
	vars[0] = malloc (sizeof(t_var));
	vars[1] = malloc (sizeof(t_var));
	vars[2] = malloc (sizeof(t_var));
	vars[3] = malloc (sizeof(t_var));
	vars[4] = malloc (sizeof(t_var));
	vars[0]->name = ft_strdup("name0");
	vars[0]->content = ft_strdup("4567891011");
	vars[1]->name = ft_strdup("name1");
	vars[1]->content = ft_strdup("content1");
	vars[2]->name = ft_strdup("name2");
	vars[2]->content = ft_strdup("content2");
	vars[3]->name = ft_strdup("name3");
	vars[3]->content = ft_strdup("content3");
	vars[4]->name = ft_strdup("name4");
	vars[4]->content = ft_strdup("content4");
	
	set_variable(&local, vars[0]);
	set_variable(&local, vars[1]);
	set_variable(&local, vars[2]);
	set_variable(&env, vars[3]);
	set_variable(&env, vars[4]);
	print_list(local);
	print_list(env);
	char *expanded = expand_variables(v[1], local, env);
	printf("expanded: %s\n",expanded);
	ft_lstfree(&local, free_var);
	ft_lstfree(&env, free_var);
	free(vars);
	return (1);
}*/