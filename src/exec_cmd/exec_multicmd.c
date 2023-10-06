/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_multicmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 18:40:15 by iortega-          #+#    #+#             */
/*   Updated: 2023/10/06 12:38:34 by iortega-         ###   ########.fr       */
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

static void	middle_child(t_multicmd *data, t_cmd *cmds, t_list **varlist)
{
	close(data->new_pipe[0]);
	if (cmds->infile < 0 || cmds->outfile < 0)
		exit (1);
	if (cmds->infile > 0)
	{
		if (cmds->outfile > 1)
			child(cmds->infile, cmds->outfile,
				(char **)cmds->args, varlist);
		else
			child(cmds->infile, data->new_pipe[1],
				(char **)cmds->args, varlist);
	}
	else if (cmds->outfile > 1)
		child(data->last_pipe[0], cmds->outfile,
			(char **)cmds->args, varlist);
	else
		child(data->last_pipe[0], data->new_pipe[1],
			(char **)cmds->args, varlist);
}

static void	manage_child(t_multicmd *data, t_cmd *cmds, t_list **varlist)
{
	if (data->pid == 0)
		middle_child(data, cmds, varlist);
	else
	{
		close(data->last_pipe[0]);
		data->last_pipe[0] = data->new_pipe[0];
		data->last_pipe[1] = data->new_pipe[1];
		if (cmds->infile > 0)
			close(cmds->infile);
		if (cmds->outfile > 1)
			close(cmds->outfile);
	}
}

static int	middle_cmds(t_multicmd *data, t_list **cmds, t_list **varlist)
{
	data->i = 0;
	while (++data->i < data->cmds_len - 1)
	{
		if (pipe(data->new_pipe) == -1)
		{
			printf("EXEC_MULTI_CMD: pipe %d fail\n", data->i);
			return (1);
		}
		close(data->last_pipe[1]);
		data->pid = fork();
		if (data->pid == -1)
		{
			printf("EXEC_MULTI_CMD: fork %d: fail\n", data->i);
			return (1);
		}
		manage_child(data, ((t_cmd *)(*cmds)->content), varlist);
		*cmds = (*cmds)->next;
	}
	return (0);
}

static int	last_cmd(t_multicmd *data, t_cmd *cmds, t_list **varlist)
{
	data->pid = fork();
	if (data->pid == 0)
	{
		if (cmds->infile < 0 || cmds->outfile < 0)
			exit (1);
		if (cmds->infile > 0)
			child(cmds->infile, cmds->outfile, (char **)cmds->args, varlist);
		else
			child(data->last_pipe[0], cmds->outfile,
				(char **)cmds->args, varlist);
	}
	else
	{
		if (cmds->infile > 0)
			close(cmds->infile);
		if (cmds->outfile > 1)
			close(cmds->outfile);
	}
	close (data->last_pipe[0]);
	return (data->pid);
}

int	exec_multi_cmd(t_list *cmds, t_list **varlist)
{
	t_multicmd	data;
	pid_t		last_pid;

	data.cmds_len = ft_lstsize(cmds);
	if (pipe(data.last_pipe) == -1)
	{
		printf("EXEC_MULTI_CMD: pipe 0: fail\n");
		return (1);
	}
	if (first_cmd(&data, (t_cmd *)cmds->content, varlist))
		return (1);
	cmds = cmds->next;
	if (middle_cmds(&data, &cmds, varlist))
		return (1);
	close(data.last_pipe[1]);
	last_pid = last_cmd(&data, (t_cmd *)cmds->content, varlist);
	waitpid(last_pid, &(data.status), 0);
	while (wait(NULL) != -1)
		;
	return (WEXITSTATUS(data.status));
}
