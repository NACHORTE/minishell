/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 21:23:14 by orudek            #+#    #+#             */
/*   Updated: 2023/09/27 21:23:54 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "t_var.h"

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
