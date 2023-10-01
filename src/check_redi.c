/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 15:45:49 by iortega-          #+#    #+#             */
/*   Updated: 2023/10/01 15:52:33 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	just_redi(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '<' && str[i] != '>')
			return (0);
		i++;
	}
	return (1);
}

static int	no_redi(char **cmd)
{
	int	i;
	int	j;

	i = 0;
	while (cmd[i])
	{
		j = 0;
		while (cmd[i][j])
		{
			if (cmd[i][j] != '>' && cmd[i][j] != '<')
				break ;
			j++;
			if (!cmd[i][j])
			{
				printf("syntax error near unexpected token `newline'\n");
				return (1);
			}
		}
		i++;
	}
	return (0);
}

static int	check_redi(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (just_redi(cmd[i]))
		{
			if ((cmd[i][0] == '>' || cmd[i][0] == '<') && cmd[i + 1])
			{
				if (cmd[i + 1][0] == '>' || cmd[i + 1][0] == '<')
				{
					printf("syntax error near unexpected token '%c'\n",
						cmd[i + 1][0]);
					return (0);
				}
			}
		}
		i++;
	}
	if (no_redi(cmd))
		return (0);
	return (1);
}

int	redirect_ok(t_list *cmds)
{
	t_list	*aux;

	aux = cmds;
	while (aux)
	{
		if (!check_redi((char **)aux->content))
			return (0);
		aux = aux->next;
	}
	return (1);
}
