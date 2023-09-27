/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 12:22:38 by orudek            #+#    #+#             */
/*   Updated: 2023/09/27 21:26:21 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd) //NOTE no se si me gusta está funcion.
{
	if (!cmd || !*cmd)
		return (0);
	if (!ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "export"))
		return (1);
	return (0);
}

int	exec_builtin(char **cmd, t_list **local, t_list **env)
{
	if (!cmd || !*cmd)
		return (1);
	if (!ft_strcmp(cmd[0], "pwd"))
		return (cmd_pwd());
	else if (!ft_strcmp(cmd[0], "cd"))
		return (cmd_cd(cmd, env));
	else if (!ft_strcmp(cmd[0], "env"))
		return (cmd_env(*env));
	else if (!ft_strcmp(cmd[0], "echo"))
		return (cmd_echo(cmd));
	else if (!ft_strcmp(cmd[0], "unset"))
		return (cmd_unset(cmd, local, env));
	else if (!ft_strcmp(cmd[0], "exit"))
		cmd_exit();
	else if (!ft_strcmp(cmd[0], "export"))
		return (cmd_export(cmd, local, env));
	return (1);
}
