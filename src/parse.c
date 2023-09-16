/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 14:14:20 by orudek            #+#    #+#             */
/*   Updated: 2023/09/16 18:49:53 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**split_pipe(char *s);
char	*expand_variables(char *str, t_list *local, t_list *env);
char	**split_args(char const *s, char c);

char	**expand_vars_array(char **str, t_list *local, t_list *env)
{
	int		i;
	char	**output;
	char	len;

	len = 0;
	while (str[len])
		len++;
	output = malloc(sizeof(char *) * (len + 1));
	if (!output)
		return (0);
	i = -1;	
	while (str[++i])
	{
		output[i] = expand_variables(str[i], local, env);
		if (!output[i])
		{
			ft_array_free(output);
			return (0);
		}
	}
	output[i] = 0;
	return (output);
}

void	free_list(t_list *lst)
{
	t_list	*next;

	while (lst)
	{
		next = lst->next;
		ft_array_free((char**)lst->content);
		lst = next;
	}
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
			free_list(output);
			return (0);	
		}
		array++;
	}
	return (output);
}

t_list	*parse(char	*input, t_list *local, t_list *env)
{
	char	**split_pipes;
	char	**expanded;

	split_pipes = split_pipe(input);
	if (!split_pipes)
		return (NULL);
	expanded = expand_vars_array(split_pipes, local, env);
	if (!expanded)
		return (NULL);
	return (save_commands(expanded));
}

/*
int main (int c, char **v)
{
	if (c == 1)
		return 1;
	t_list *parsed = parse(v[1], NULL, NULL);
	int j = 0;
	while (parsed)
	{
		printf("List %d\n",j++);
		int i = 0;
		for (i = 0; ((char **)parsed->content)[i]; i++)
			printf("\tstr[%d]=%s$\n",i,((char **)parsed->content)[i]);
		printf("\tstr[%d]=%s$\n",i,((char **)parsed->content)[i]);
		parsed = parsed->next;
	}
}*/
