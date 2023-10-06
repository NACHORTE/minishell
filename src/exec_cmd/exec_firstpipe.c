/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_firstpipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:32:32 by iortega-          #+#    #+#             */
/*   Updated: 2023/10/06 12:35:01 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_cmd.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/wait.h>

#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

static void	child_first(t_multicmd *data, t_cmd *cmds, t_list **varlist)
{
	close(data->last_pipe[0]);
	if (cmds->infile < 0 || cmds->outfile < 0)
		exit (1);
	if (cmds->outfile > 1)
		child (cmds->infile, cmds->outfile, (char **)cmds->args, varlist);
	else
		child (cmds->infile, data->last_pipe[1],
			(char **)cmds->args, varlist);
}

int	first_cmd(t_multicmd *data, t_cmd *cmds, t_list **varlist)
{
	data->i = 0;
	data->pid = fork();
	if (data->pid == -1)
	{
		printf("EXEC_MULTI_CMD: fork 0: fail\n");
		return (1);
	}
	if (data->pid == 0)
		child_first(data, cmds, varlist);
	else
	{
		if (cmds->infile > 0)
			close(cmds->infile);
		if (cmds->outfile > 1)
			close(cmds->outfile);
	}
	return (0);
}
