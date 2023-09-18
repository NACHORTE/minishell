/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 17:24:03 by orudek            #+#    #+#             */
/*   Updated: 2023/09/17 17:36:08 by iortega-         ###   ########.fr       */
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

t_list	*ft_parse(char	*input, t_list *local, t_list *env)
{
	char	**split_pipes;
	char	**expanded;
	t_list	*out;

	split_pipes = split_pipe(input);
	if (!split_pipes)
		return (NULL);
	expanded = expand_vars_array(split_pipes, local, env);
	ft_array_free(split_pipes);
	if (!expanded)
		return (NULL);
	out = save_commands(expanded);
	ft_array_free(expanded);
	return (out);
}

//COMENTAR A PARTIR DE AQUI
/*
#include "t_var.h"
int save_env(t_list **list, char **envp)
{
	t_var *tmp;
	int	len;
	int	i;
	int j;
	int aux;

	i = 0;
	while (envp[i])
	{
		tmp = malloc(sizeof(t_var));
		j = 0;
		len = 0;
		while (envp[i][j] != '=')
		{
			len++;
			j++;
		}
		tmp->name = malloc(sizeof(char) * (len + 1));
		ft_strlcpy(tmp->name, envp[i], len + 1);
		j++;
		aux = j;
		len = 0;
		while (envp[i][j] != '\0')
		{
			len++;
			j++;
		}
		tmp->content = malloc(sizeof(char) * (len + 1));
		ft_strlcpy(tmp->content, &envp[i][aux], len + 1);
		set_variable(list, tmp);
		i++;
	}
	return (0);
}
int main (int c, char **v, char **env)
{
	if (c == 1)
		return 1;
	t_list	*env_list;
	save_env(&env_list, env);
	t_list	*aux;
	aux = env_list;
	t_list *parsed = parse(v[1], NULL, env_list);
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