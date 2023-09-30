/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 21:23:14 by orudek            #+#    #+#             */
/*   Updated: 2023/09/28 22:19:23 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_cmd.h"

int	cmd_unset(char **cmd, t_list **varlist)
{
	while (*(++cmd))
	{
		if (is_in_varlist(*varlist, *cmd) != -1)
			unset_variable(varlist, *cmd);
	}
	return (0);
}
