/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_heredocs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 18:42:20 by iortega-          #+#    #+#             */
/*   Updated: 2023/09/30 18:50:56 by iortega-         ###   ########.fr       */
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

static void	error_heredoc(int **fds)
{
	int	i;

	i = 0;
	while ((*fds)[i] != -1)
	{
		close((*fds)[i]);
		i++;
	}
}

static int	count_heredoc(t_list *cmds)
{
	int	i;
	int	count;

	count = 0;
	while (cmds)
	{
		i = 0;
		while (((char **)cmds->content)[i])
		{
			if (!ft_strncmp(((char **)cmds->content)[i], "<<", 2))
				count++;
			i++;
		}
		cmds = cmds->next;
	}
	return (count);
}

static int	open_heredocs(t_list *cmds, int **n_cmd, int **fds)
{
	int	fd;
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cmds)
	{
		fd = 0;
		fd = check_restdin_here((char **)cmds->content);
		if (fd != 0)
		{
			(*n_cmd)[j] = i;
			(*fds)[j] = fd;
			j++;
		}
		if (fd == -1)
		{
			error_heredoc(fds);
			return (1);
		}
		i++;
		cmds = cmds->next;
	}
	return (0);
}

int	pipe_heredocs(t_multicmd *data, t_list *cmds)
{
	data->n_heredocs = count_heredoc(cmds);
	if (data->n_heredocs > 0)
	{
		data->n_cmd = malloc(sizeof(int) * data->n_heredocs);
		data->fds = malloc(sizeof(int) * data->n_heredocs);
		if (open_heredocs(cmds, &(data->n_cmd), &(data->fds)))
		{
			free(data->n_cmd);
			free(data->fds);
			return (1);
		}
	}
	return (0);
}
