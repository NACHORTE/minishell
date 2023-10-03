/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 17:24:03 by orudek            #+#    #+#             */
/*   Updated: 2023/10/03 13:28:24 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**expand_vars_array(char **str, t_list *varlist)
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
}

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

t_arg_redir	*arg_redir(char **cmds)
{
	t_arg_redir	*out;
	int	i;
	int	j;
	int	quotes;
	int	simples;
	char	*str;

	out = malloc(sizeof(t_arg_redir));
	if (!out)
		return (NULL);
	i = 0;
	quotes = 0;
	simples = 0;
	str = NULL;
	while (cmds[i])
	{
		j = 0;
		while (cmds[i][j])
		{
			if (cmds[i][j] == '\'' && !quotes)
				simples = !simples;
			else if (cmds[i][j] == '"' && !simples)
				quotes = !quotes;
			if (!quotes && !simples)
			{
				if (cmds[i][j] == '<' || cmds[i][j] == '>')
				{
					while (cmds[i][j] == ' ')
						j++;
					while (cmds[i][j] && cmds[i][j] != ' ')
					{
						if (cmds[i][j] == '\'')
						{
							while (cmds[i][j] != '\'')
							{
								
							}
						}
						else if (cmds[i][j] == '"')
						{
							
						}
						
					}
				}
			}
		}
	}
}

t_list	*parse(char	*input, t_list *varlist)
{
	char	**split_pipes;
	char	**expanded;
	t_list	*out;

	if (!input)
		return (NULL);
	split_pipes = split_pipe(input);
	if (!split_pipes)
		return (NULL);
	expanded = expand_vars_array(split_pipes, varlist);
	ft_array_free(split_pipes);
	if (!expanded)
		return (NULL);
	out = save_commands(expanded);
	ft_array_free(expanded);
	return (out);
}
