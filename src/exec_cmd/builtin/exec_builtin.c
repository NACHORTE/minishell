/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 12:22:38 by orudek            #+#    #+#             */
/*   Updated: 2023/10/02 14:20:53 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_cmd.h"

int	exec_builtin(char **cmd, t_list **varlist)
{
	if (!cmd || !*cmd)
		return (1);
	if (!ft_strcmp(cmd[0], "pwd"))
		return (cmd_pwd());
	else if (!ft_strcmp(cmd[0], "cd"))
		return (cmd_cd(cmd, varlist));
	else if (!ft_strcmp(cmd[0], "env"))
		return (cmd_env(*varlist));
	else if (!ft_strcmp(cmd[0], "echo"))
		return (cmd_echo(cmd));
	else if (!ft_strcmp(cmd[0], "unset"))
		return (cmd_unset(cmd, varlist));
	else if (!ft_strcmp(cmd[0], "exit"))
		return (cmd_exit(cmd, varlist));
	else if (!ft_strcmp(cmd[0], "export"))
		return (cmd_export(cmd, varlist));
	return (1);
}
