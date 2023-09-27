/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 15:58:32 by iortega-          #+#    #+#             */
/*   Updated: 2023/09/27 22:49:51 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <sys/types.h>
# include <sys/wait.h>
# include <sys/uio.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <stdlib.h>
# include <fcntl.h>
# include "libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include "t_var.h"

typedef struct s_command
{
	t_list	*env;
	t_list	*local;
	t_list	*cmds;
	int		last_status;
}t_command;

char	**split_args(char const *str, char c);
t_list	*parse(char	*input, t_list *local, t_list *env);
int    exec_cmd(t_list *cmds, t_list *env, t_command *global);
void    child(int infile, int outfile, char ** cmd, t_command *global);
int	is_builtin(char *cmd);
int	exec_builtin(char **cmd, t_list **local, t_list **env);
void	redirect_streams(int infile, int outfile, char **cmd);
char	**parse_cmd(char **input);

#endif