/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 22:31:08 by orudek            #+#    #+#             */
/*   Updated: 2023/10/06 15:40:01 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_CMD_H
# define EXEC_CMD_H

# include "t_var.h"
# include "libft.h"
# include "parse.h"
# include "minishell.h"

typedef struct s_multicmd
{
	int	i;
	int	last_pipe[2];
	int	new_pipe[2];
	int	pid;
	int	cmds_len;
	int	status;
	int	*n_cmd;
	int	*fds;
	int	n_heredocs;
	int	j;
}	t_multicmd;

int		cmd_cd(char **cmd, t_list **varlist);
int		cmd_echo(char **cmd);
int		cmd_env(t_list *varlist);
int		cmd_exit(char **cmd, t_list **varlist);
int		cmd_export(char **cmd, t_list **varlist);
int		cmd_pwd(void);
int		cmd_unset(char **cmd, t_list **varlist);

int		exec_cmd(t_list *cmds, t_list **varlist);
int		is_builtin(char *cmd);
int		exec_builtin(char **cmd, t_list **varlist);
void	child(int infile, int outfile, char **cmd, t_list **varlist);
void	redirect_streams(int infile, int outfile, char **cmd);
int		exec_multi_cmd(t_list *cmds, t_list **varlist);
int		pipe_heredocs(t_multicmd *data, t_list *cmds);
int		here_doc(char *str);
int		check_restdin_here(char **input);
int		first_cmd(t_multicmd *data, t_cmd *cmds, t_list **varlist);
char	**rm_assignation(char **cmd);
void	redirect(int infile, int outfile);

#endif
