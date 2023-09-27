/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 21:23:14 by orudek            #+#    #+#             */
/*   Updated: 2023/09/27 22:39:59 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_cmd.h"

int	cmd_unset(char **cmd, t_list **local, t_list **env)
{
	while (*cmd)
	{
		if (get_variable(*local, cmd[0], NULL) == 1)
			unset_variable(local, cmd[0]);
		if (get_variable(*env, cmd[0], NULL) == 1)
			unset_variable(env, cmd[0]);
		cmd++;
	}
	return (0);
}
