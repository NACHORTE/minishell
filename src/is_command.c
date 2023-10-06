/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 16:30:39 by iortega-          #+#    #+#             */
/*   Updated: 2023/10/06 13:33:09 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	inval_char(int quotes, int simple, char c)
{
	if ((quotes % 2 == 0 && simple % 2 == 0)
		&& (c == '\\' || c == ';'))
	{
		printf("Invalid character: \"%c\"\n", c);
		return (1);
	}
	return (0);
}

int	check_closed_quotes(char *input)
{
	int	i;
	int	quotes;
	int	simple;

	i = 0;
	quotes = 0;
	simple = 0;
	while (input[i])
	{
		if (input[i] == '\'' && quotes % 2 == 0)
			simple++;
		else if (input[i] == '"' && simple % 2 == 0)
			quotes++;
		if (inval_char(quotes, simple, input[i]))
			return (0);
		i++;
	}
	if (quotes % 2 == 0 && simple % 2 == 0)
		return (1);
	else
	{
		printf("Please close quotes.\n");
		return (0);
	}
}

static int	local_declare(t_command *global)
{
	if (ft_lstsize(global->cmds) == 1
		&& is_allasignation((t_cmd *)global->cmds->content))
	{
		save_variables(global);
		return (1);
	}
	return (0);
}

int	is_command(t_command *global)
{
	if (!global->cmds || !((char **)((t_cmd *)global->cmds->content)->args)[0])
		return (0);
	else if (local_declare(global))
		return (0);
	else
		return (1);
}
