/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 21:22:08 by orudek            #+#    #+#             */
/*   Updated: 2023/10/02 14:45:18 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "libft.h"
#include "t_var.h"

char	is_number(char *str)
{
	if (*str == '+' || *str == '-')
		str++;
	if (!ft_isdigit(*(str++)))
		return (0);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	get_exit_status(t_list *varlist, int *exit_status)
{
	t_var	*var;

	while (varlist)
	{
		var = varlist->content;
		if (!ft_strcmp(var->name, "?"))
		{
			*exit_status = ft_atoi(var->content);
			return (1);
		}
		varlist = varlist->next;
	}
	return (0);
}

int	cmd_exit(char **cmd, t_list **varlist)
{
	int exit_status;

	printf("exit\n");
	if (!cmd[1])
	{
		if (!get_exit_status(*varlist, &exit_status))
		{
			printf("Couldn't find $?\n");
			return (1);
		}
		exit(exit_status);
	}
	if (!is_number(cmd[1]))
	{
		write(2, "minishell: exit: ", 17);
		write(2, cmd[1], ft_strlen(cmd[1]));
		write(2, ": numeric argument required\n", 28);
		//printf("minishell: exit: %s: numeric argument required\n", cmd[1]);
		exit(2);
	}
	if (cmd[2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		//printf("minishell: exit: too many arguments\n");
		return (1);
	}
	exit(ft_atoi(cmd[1]));
}
