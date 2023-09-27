/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 22:31:08 by orudek            #+#    #+#             */
/*   Updated: 2023/09/27 22:48:14 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_CMD_H
# define EXEC_CMD_H

#include "libft.h"
#include "t_var.h"

int	cmd_cd(char **cmd, t_list **env);
int	cmd_echo(char **cmd);
int	cmd_env(t_list *env);
int	cmd_exit();
int	cmd_export(char **cmd, t_list **local, t_list **env);
int	cmd_pwd();
int	cmd_unset(char **cmd, t_list **local, t_list **env);

#endif
