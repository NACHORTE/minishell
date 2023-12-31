/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 15:58:32 by iortega-          #+#    #+#             */
/*   Updated: 2023/10/06 13:22:18 by iortega-         ###   ########.fr       */
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
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include "libft.h"
# include "t_var.h"
# include "exec_cmd.h"
# include "parse.h"

typedef struct s_command
{
	t_list	*varlist;
	t_list	*cmds;
	int		last_status;
}	t_command;

int		redirect_ok(t_list *cmds);
int		save_variables(t_command *global);
int		is_allasignation(t_cmd *cmds);
int		is_command(t_command *global);
int		check_closed_quotes(char *input);
void	new_line(int sig);
void	sig_child(int sig);
void	sig_here(int sig);

#endif