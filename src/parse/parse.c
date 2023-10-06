/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 17:24:03 by orudek            #+#    #+#             */
/*   Updated: 2023/10/06 13:34:39 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

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
