/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 22:37:17 by orudek            #+#    #+#             */
/*   Updated: 2023/09/18 23:16:27 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "t_var.h"

static char	find_var(char **var, char *str, int len, t_list *list)
{
	int	i;

	if (*str == '$')
		str++;
	while (list)
	{
		i = -1;
		while (++i < len)
		{
			if (str[i] != ((t_var *)list->content)->name[i])
				break ;
			if (i == len - 1 && ((t_var *)list->content)->name[i + 1] == '\0')
			{
				*var = ((t_var *)list->content)->content;
				return (1);
			}
		}
		list = list->next;
	}
	*var = NULL;
	return (0);
}

static int	arg_len(char *s, int *len, t_list *local, t_list *env)
{
	int		i;
	char	*var;

	i = 0;
	while (s[++i])
	{
		if (s[i] == ' ' || s[i] == '"' || s[i] == '\'' || s[i] == '$')
		{
			if (i == 1 && s[i] == '$')
				i++;
			break ;
		}
	}
	if (i == 1)
	{
		(*len) += 1;
		return (i);
	}
	if (!find_var(&var, s, i - 1, local) && !find_var(&var, s, i - 1, env))
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
		if (state == 0 && (*str == '\'' || *str == '"'))
			state = (*str == '\'') * 2 + (*str == '"');
		else if ((state == 1 && *str == '"') || (state == 2 && *str == '\''))
			state = 0;
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

static int	dup_arg(char *out, char **s, t_list *local, t_list *env)
{
	int		len;
	int		j;
	int		i;
	char	*var;

	len = 0;
	i = arg_len(*s, &len, local, env);
	if (len == 0)
	{
		(*s) += i;
		return (0);
	}
	if (i == 1)
	{
		(*s) += i;
		*out = '$';
		return (1);
	}
	if (!find_var(&var, *s, i - 1, local) && !find_var(&var, *s, i - 1, env))
		return (i);
	j = -1;
	while (++j < len)
		*out++ = *var++;
	(*s) += i;
	return (len);
}

char	*expand_variables(char *str, t_list *local, t_list *env)
{
	char	*out;
	int		len;
	int		i;
	int		state;

	len = expanded_len(str, local, env);
	out = malloc(len + 1);
	if (!out)
		return (NULL);
	i = 0;
	state = 0;
	while (i < len)
	{
		if (state == 0 && (*str == '\'' || *str == '"'))
			state = (*str == '\'') * 2 + (*str == '"');
		else if ((state == 1 && *str == '"') || (state == 2 && *str == '\''))
			state = 0;
		if (state != 2 && *str == '$')
			i += dup_arg(&out[i], &str, local, env);
		else
			out[i++] = *str++;
	}
	out[i] = 0;
	return (out);
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
	vars[1]->name = ft_strdup("$");
	vars[1]->content = ft_strdup("content1");
	vars[2]->name = ft_strdup("name2");
	vars[2]->content = ft_strdup("content2");
	vars[3]->name = ft_strdup("name3");
	vars[3]->content = ft_strdup("content3");
	vars[4]->name = ft_strdup("name4");
	vars[4]->content = ft_strdup("content4");
	
	set_variable(&local, vars[0]->name, vars[0]->content);
	set_variable(&local, vars[1]->name, vars[1]->content);
	set_variable(&local, vars[2]->name, vars[2]->content);
	set_variable(&env, vars[3]->name, vars[3]->content);
	set_variable(&env, vars[4]->name, vars[4]->content);
	print_list(local);
	print_list(env);
	char *expanded = expand_variables(v[1], local, env);
	printf("expanded: \"%s\"\n",expanded);
	ft_lstfree(local, free_var);
	ft_lstfree(env, free_var);
	free(vars);
	return (0);
}*/
