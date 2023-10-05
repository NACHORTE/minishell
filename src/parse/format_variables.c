/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 22:37:17 by orudek            #+#    #+#             */
/*   Updated: 2023/10/05 18:55:52 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "t_var.h"

static char	*format_redir(char *s)
{
	int	state;
	char *out;
	int	i;

	*out = malloc(arg_len(s) + 1);
	if (!*out)
		return (NULL);
	state = 0;
	i = 0;
	s--;
	while (*++s)
	{
		if (state == 0 && (*s == '\'' || *s == '"'))
		{
			state = (*s == '\'') * 2 + (*s == '"');
			continue ;
		}
		else if ((state == 1 && *s == '"') || (state == 2 && *s == '\''))
		{
			state = 0;
			continue ;
		}
		out[i++] = *s;
	}
	out[i] = '\0';
	return (out);
}

static int	arg_len(char *s)
{
	int	len;

	len = 0;
	str--;
	while (*++str)
	{
		if (state == 0 && (*s == '\'' || *s == '"'))
		{
			state = (*s == '\'') * 2 + (*s == '"');
			continue ;
		}
		else if ((state == 1 && *s == '"') || (state == 2 && *s == '\''))
		{
			state = 0;
			continue ;
		}
		len++;
	}
	return (len);
}

static char	*format_args(char *s)
{
	char *out;
	int	i;

	*out = malloc(arg_len(s) + 1);
	if (!*out)
		return (NULL);
	i[0] = 0;
	i[1] = 0;
	s--;
	while (*++s)
	{
		if (i[0] == 0 && (*s == '\'' || *s == '"'))
		{
			i[0] = (*s == '\'') * 2 + (*s == '"');
			continue ;
		}
		else if ((i[0] == 1 && *s == '"') || (i[0] == 2 && *s == '\''))
		{
			i[0] = 0;
			continue ;
		}
		out[i[1]++] = *s;
	}
	out[i[1]] = '\0';
	return (out);
}

void	pop_arg(t_list **lst, t_list **aux)
{
	t_list	*previous_lst;
	
	if (aux == *lst)
	{
		*lst = (*lst)->next;
		ft_lstdelone(*aux, free);
		aux = *lst;
		return ;
	}
	previous_lst = *lst;
	while (previous_lst->next != *aux)
		previous_lst = previous_lst->next;
	previous_lst->next = aux->next;
	ft_lstdelone(aux, free);
	*aux = previous_lst->next;
}

static int	format_list(t_list **lst, t_list *(*format_str)(char *))
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
