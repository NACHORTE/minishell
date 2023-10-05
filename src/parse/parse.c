/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 17:24:03 by orudek            #+#    #+#             */
/*   Updated: 2023/10/05 21:45:38 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*save_commands(char **array)
{
	t_list	*output;
	char	**args;

	output = NULL;
	while (*array)
	{
		args = split_args(*array, ' ');
		if (!args || !ft_lstadd_back_content(&output, args))
		{
			ft_lstfree(output, ft_array_free);
			return (0);
		}
		array++;
	}
	return (output);
}

void print_arg(t_list *arg_redir_lst)
{
	t_list *aux;

	aux = (t_list *)((t_arg_redir *)arg_redir_lst->content)->args;
	while (aux)
	{
		printf("%s\n", (char *)aux->content);
		aux = aux->next;
	}
}

t_list	*parse(char	*input, t_list *varlist)
{
	char	**split_pipes;
	t_list	*arg_redir_lst;
	t_list	*out;

	if (!input)
		return (NULL);
	split_pipes = split_pipe(input);
	if (!split_pipes)
		return (NULL);
	arg_redir_lst = split_arg_redir(split_pipes);
	ft_array_free(split_pipes);
	if (!expand_variables(arg_redir_lst, varlist)
		|| !format_variables(arg_redir_lst))
	{
		
		ft_lstfree(arg_redir_lst, free_arg_redir);
		return (NULL);
	}
	out = cmd_redir(arg_redir_lst);
	ft_lstfree(arg_redir_lst, free_arg_redir);
	return (out);
}
