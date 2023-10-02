/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_multicmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 18:40:15 by iortega-          #+#    #+#             */
/*   Updated: 2023/10/02 15:59:29 by iortega-         ###   ########.fr       */
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

static int	first_cmd(t_multicmd *data, t_list *cmds, t_list **varlist)
{
	data->i = 0;
	data->j = 0;
	data->pid = fork();
	if (data->pid == -1)
	{
		printf("EXEC_MULTI_CMD: fork 0: fail\n");
		return (1);
	}
	if (data->pid == 0)
	{
		close(data->last_pipe[0]);
		if (data->j < data->n_heredocs && data->i == data->n_cmd[data->j])
			child(data->fds[data->j], data->last_pipe[1],
				(char **)cmds->content, varlist);
		else
			child(0, data->last_pipe[1], (char **)cmds->content, varlist);
	}
	else if (data->j < data->n_heredocs && data->i == data->n_cmd[data->j])
	{
		if (data->fds[data->j] > 0)
			close(data->fds[data->j]);
		data->j++;
	}
	return (0);
}

static void	manage_child(t_multicmd *data, t_list *cmds, t_list **varlist)
{
	if (data->pid == 0)
	{
		close(data->new_pipe[0]);
		if (data->j < data->n_heredocs && data->i == data->n_cmd[data->j])
			child(data->fds[data->j], data->new_pipe[1],
				(char **)cmds->content, varlist);
		else
			child(data->last_pipe[0], data->new_pipe[1],
				(char **)cmds->content, varlist);
	}
	else
	{
		close(data->last_pipe[0]);
		data->last_pipe[0] = data->new_pipe[0];
		data->last_pipe[1] = data->new_pipe[1];
		if (data->j < data->n_heredocs && data->i == data->n_cmd[data->j])
		{
			if (data->fds[data->j] > 0)
				close(data->fds[data->j]);
			data->j++;
		}
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
		manage_child(data, *cmds, varlist);
		*cmds = (*cmds)->next;
	}
	return (0);
}

static int	last_cmd(t_multicmd *data, t_list *cmds, t_list **varlist)
{
	data->pid = fork();
	if (data->pid == 0)
	{
		if (data->j < data->n_heredocs && data->i == data->n_cmd[data->j])
		{
			child(data->fds[data->j], 1, (char **)cmds->content, varlist);
		}
		else
		{
			child(data->last_pipe[0], 1, (char **)cmds->content, varlist);
		}
	}
	else if (data->j < data->n_heredocs && data->i == data->n_cmd[data->j])
	{
		if (data->fds[data->j] > 0)
			close(data->fds[data->j]);
		data->j++;
	}
	close (data->last_pipe[0]);
	return (data->pid);
}

int	exec_multi_cmd(t_list *cmds, t_list **varlist)
{
	t_multicmd	data;
	pid_t		last_pid;

	data.cmds_len = ft_lstsize(cmds);
	if (pipe_heredocs(&data, cmds))
		return (1);
	if (pipe(data.last_pipe) == -1)
	{
		printf("EXEC_MULTI_CMD: pipe 0: fail\n");
		return (1);
	}
	if (first_cmd(&data, cmds, varlist))
		return (1);
	cmds = cmds->next;
	if (middle_cmds(&data, &cmds, varlist))
		return (1);
	close(data.last_pipe[1]);
	last_pid = last_cmd(&data, cmds, varlist);
	waitpid(last_pid, &(data.status), 0);
	while (wait(NULL) != -1)
		;
	if (data.n_heredocs > 0)
	{
		free (data.n_cmd);
		free(data.fds);
	}
	return (WEXITSTATUS(data.status));
}
