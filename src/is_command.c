/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_command.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 16:30:39 by iortega-          #+#    #+#             */
/*   Updated: 2023/10/01 16:32:57 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_closed_quotes(char *input)
{
	int	i;
	int	quotes;

	i = 0;
	quotes = 0;
	while (input[i])
	{
		if (input[i] == '"')
			quotes++;
		if ((quotes % 2 == 0) && (input[i] == '\\' || input[i] == ';'))
		{
			printf("Invalid character: \"%c\"\n", input[i]);
			return (0);
		}
		i++;
	}
	if (quotes % 2 == 0)
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
		&& is_allasignation((char **)global->cmds->content))
	{
		save_variables(global);
		return (1);
	}
	return (0);
}

int	is_command(t_command *global)
{
	if (!global->cmds || !((char **)global->cmds->content)[0])
		return (0);
	else if (local_declare(global))
		return (0);
	else if (!redirect_ok(global->cmds))
		return (0);
	else
		return (1);
}
