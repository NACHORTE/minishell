/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 22:37:17 by orudek            #+#    #+#             */
/*   Updated: 2023/10/05 20:38:30 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "t_var.h"

static int	is_end_of_var(char c)
{
	if (c == ' ' || c == '\"' || c == '\'' || c == '=' || c == '<' || c == '>'
		|| c == '\0')
		return (1);
	return (0);
}

static char	find_var(char **var, char *str, int len, t_list *list)
{
	int	i;

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
	return (0);
}

static int	var_len(char *s, int *len, t_list *varlist)
{
	int		i;
	char	*var;

	i = 0;
	while (s[i] && !is_end_of_var(s[i]))
		i++;
	if (!find_var(&var, s, i - 1, varlist))
		return (i);
	*len += ft_strlen(var);
	return (i);
}

static int	expanded_len(char *str, t_list *varlist)
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
		if (state != 2 && *str == '$' && !is_end_of_var(*(str + 1)))
			str += var_len(str, &len, varlist);
		else
		{
			len++;
			str++;
		}
	}
	return (len);
}

static int	cat_arg(char *out, char **s, t_list *varlist)
{
	int		len;
	int		j;
	int		i;
	char	*var;

	len = 0;
	i = var_len(*s, &len, varlist);
	find_var(&var, *s, i - 1, varlist);
	j = -1;
	while (++j < len)
		*out++ = *var++;
	(*s) += i;
	return (len);
}

static char	*get_expanded_str(char *str, t_list *varlist)
{
	char	*out;
	int		len;
	int		i;
	int		state;

	len = expanded_len(str, varlist);
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
		if (state != 2 && *str == '$' && !is_end_of_var(*(str + 1)))
			i += cat_arg(&out[i], &str, varlist);
		else
			out[i++] = *str++;
	}
	out[i] = 0;
	return (out);
}

//////////////////////////////////////////////////////////////////////////////////////
static int	arg_len(char *s)
{
	int	i;
	int	state;

	i = -1;
	state = 0;
	while (s[++i]
		&& (state || (s[i] != ' ' && s[i] != '<' && s[i] != '>')))
	{
		if (state == 0 && (s[i] == '\'' || s[i] == '"'))
			state = (s[i] == '\'') * 2 + (s[i] == '"');
		else if ((state == 1 && s[i] == '"') || (state == 2 && s[i] == '\''))
			state = 0;
	}
	return (i);
}

static char	*get_arg(char **cmd)
{
	int		len;
	char	*out;

	len = arg_len(*cmd);
	out = malloc(sizeof(char) * (len + 1));
	if (!out)
		return (NULL);
	ft_strlcpy(out, *cmd, len + 1);
	(*cmd) += len;
	return (out);
}

static t_list	*str_to_arg(char *str)
{
	t_list	*out;
	char	*arg;

	out = NULL;
	while (1)
	{
		while (*str == ' ')
			str++;
		arg = get_arg(&str);
		if (!arg || !ft_lstadd_back_content(&out, arg))
		{
			ft_lstfree(out, free);
			if (arg)
				free(arg);
			return ((void *)return_msg("Couldn't allocate memory", 2, 0));
		}
		if (!*str)
			break ;
	}
	return (out);
}
//////////////////////////////////////////////////////////////////////////////////////

static int	redir_format_ok(char *s)
{
	int	state;

	state = 0;
	if (*s != '<' && *s != '>')
		return (0);
	s++;
	if (*s == '<' || *s == '>')
		s++;
	while (*s == ' ')
		s++;
	while (*s)
	{
		if (state == 0 && (*s == '\'' || *s == '"'))
			state = (*s == '\'') * 2 + (*s == '"');
		else if ((state == 1 && *s == '"') || (state == 2 && *s == '\''))
			state = 0;
		if (state == 0 && *s == ' ')
			return (return_msg("Ambiguous redirect", 2, 0));
		s++;
	}
	return (1);
}

static t_list	*str_to_redir(char *str)
{
	t_list	*out;
	char	*new_str;

	if (!redir_format_ok(str))
		return (NULL);
	new_str = ft_strdup(str);
	if (!new_str)
		return (NULL);
	out = NULL;
	if (!ft_lstadd_back_content(&out, new_str))
		free(new_str);
	return (out);
}
//////////////////////////////////////////////////////////////////////////////////////
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
	char *expanded = expand_variables(v[1], local);
	printf("expanded: \"%s\"\n",expanded);
	ft_lstfree(local, free_var);
	ft_lstfree(env, free_var);
	free(vars);
	return (0);
}
*/
