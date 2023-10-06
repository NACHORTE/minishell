/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_declare.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 15:59:47 by iortega-          #+#    #+#             */
/*   Updated: 2023/10/06 13:28:49 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_assignation(char *cmd)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (cmd[i])
	{
		if (i != 0)
		{
			if (cmd[i] == '=')
			{
				if (cmd[i + 1])
				{
					flag = 1;
					break ;
				}
			}
		}
		i++;
	}
	return (flag);
}

int	is_allasignation(t_cmd *cmds)
{
	int		i;
	char	**args;

	i = 0;
	args = (char **)cmds->args;
	while (args[i])
	{
		if (!is_assignation(args[i]))
			return (0);
		i++;
	}
	return (1);
}

static char	*get_content(char **content, int start, int pos, char *name)
{
	int		i;
	int		size;
	char	*str;

	i = start;
	if (!is_varname_ok(name))
	{
		free(name);
		return (NULL);
	}
	size = 0;
	while (content[pos][i])
	{
		size++;
		i++;
	}
	str = malloc(sizeof(char) * (size + 1));
	if (!str)
	{
		free(name);
		return (NULL);
	}
	ft_strlcpy(str, &content[pos][start], size + 1);
	return (str);
}

static int	set_assignation(t_command *global, int pos)
{
	int		i;
	char	*name;
	char	*content;

	i = 0;
	while (((char **)((t_cmd *)global->cmds->content)->args)[pos][i] != '=')
		i++;
	i++;
	name = malloc(sizeof(char) * i);
	if (!name)
		return (0);
	ft_strlcpy(name,
		((char **)((t_cmd *)global->cmds->content)->args)[pos], i);
	content = get_content(
			((char **)((t_cmd *)global->cmds->content)->args), i, pos, name);
	if (!content)
		return (0);
	set_variable(&(global->varlist), name, content, DEFAULT_VAR);
	free(name);
	free(content);
	return (1);
}

int	save_variables(t_command *global)
{
	int	pos;

	pos = 0;
	while (((char **)((t_cmd *)global->cmds->content)->args)[pos])
	{
		if (!set_assignation(global, pos))
			printf("No memory for new variable.\n");
		pos++;
	}
	return (1);
}
