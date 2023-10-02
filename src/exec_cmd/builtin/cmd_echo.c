/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 21:24:11 by orudek            #+#    #+#             */
/*   Updated: 2023/10/02 18:53:33 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_cmd.h"

int	cmd_echo(char **cmd)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	if (!cmd[1])
	{
		printf("\n");
		return (0);
	}
	if (!ft_strcmp(cmd[1], "-n"))
	{
		flag = 1;
		i++;
	}
	while (cmd[++i])
	{
		printf("%s", cmd[i]);
		if (cmd[i + 1])
			printf(" ");
	}
	if (flag == 0)
		printf("\n");
	return (0);
}
