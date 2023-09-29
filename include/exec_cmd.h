/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 22:31:08 by orudek            #+#    #+#             */
/*   Updated: 2023/09/29 19:46:47 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_CMD_H
# define EXEC_CMD_H

#include "t_var.h"
#include "libft.h"

int	cmd_cd(char **cmd, t_list **varlist);
int	cmd_echo(char **cmd);
int	cmd_env(t_list *varlist);
int	cmd_exit(void);
int	cmd_export(char **cmd, t_list **varlist);
int	cmd_pwd(void);
int	cmd_unset(char **cmd, t_list **varlist);

int		exec_cmd(t_list *cmds, t_list **varlist);
int		is_builtin(char *cmd);
int		exec_builtin(char **cmd, t_list **varlist);
void    child(int infile, int outfile, char ** cmd, t_list **varlist);
void	redirect_streams(int infile, int outfile, char **cmd);
char	**parse_cmd(char **input);

#endif
