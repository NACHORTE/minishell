/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 17:24:03 by orudek            #+#    #+#             */
/*   Updated: 2023/10/05 18:53:08 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*char	**expand_vars_array(char **str, t_list *varlist)
{
	int		i;
	char	**output;
	int		len;

	len = 0;
	while (str[len])
		len++;
	output = malloc(sizeof(char *) * (len + 1));
	if (!output)
		return (0);
	i = -1;
	while (str[++i])
	{
		output[i] = expand_variables(str[i], varlist);
		if (!output[i])
		{
			ft_array_free(output);
			return (0);
		}
	}
	output[i] = 0;
	return (output);
}*/

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
	//char	**expanded;
	t_list	*out;

	if (!input)
		return (NULL);
	(void)(varlist);
	split_pipes = split_pipe(input);
	if (!split_pipes)
		return (NULL);
	t_list *arg_redir_lst = split_arg_redir(split_pipes);
	//print_arg(arg_redir_lst);
	if (!expand_variables(arg_redir_lst, varlist))
	{
		ft_array_free(split_pipes);
		ft_lstfree(arg_redir_lst, free_arg_redir);
		return (NULL);
	}	
	out = cmd_redir(arg_redir_lst);
	ft_array_free(split_pipes);
	ft_lstfree(arg_redir_lst, free_arg_redir);
	/*t_list *expanded_cmd = expand_vars_array(arg_redir_lst, varlist);
	ft_array_free(split_pipes);
	if (!expanded)
		return (NULL);
	out = save_commands(expanded);
	ft_array_free(expanded);*/
	return (out);
}
