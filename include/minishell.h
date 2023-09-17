/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 15:58:32 by iortega-          #+#    #+#             */
/*   Updated: 2023/09/17 18:00:40 by orudek           ###   ########.fr       */
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
	char	**path;
	char	**cmd;
	char	**cmd_parsed;
	char	*cmd_path;
	pid_t	child;
	pid_t	terminal;
	t_list	*env;
	t_list	*local;
	t_list	*cmds;
	int		sout;
}t_command;

char	**split_args(char const *str, char c);
t_list	*ft_parse(char	*input, t_list *local, t_list *env);

#endif