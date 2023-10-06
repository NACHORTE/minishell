/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_streams.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 13:11:49 by iortega-          #+#    #+#             */
/*   Updated: 2023/10/06 15:41:09 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include <fcntl.h>
#include "minishell.h"

static void	check_doublestdin(char *character, int *fd)
{
	if (*fd != 0)
		close (*fd);
	if (*character == '<')
	{
		if (*fd != 0)
			close (*fd);
		*fd = 0;
	}
	else
	{
		*fd = open(character, O_RDONLY);
		if (*fd < 0)
			perror(character);
	}
}

static int	check_restdin(t_list *redir)
{
	int		i;
	int		fd;
	char	*str;

	i = 0;
	fd = 0;
	while (fd >= 0 && redir)
	{
		str = (char *)redir->content;
		i = 0;
		if (str[i] == '<')
			check_doublestdin(&str[i + 1], &fd);
		redir = redir->next;
	}
	return (fd);
}

static void	check_doublestdout(char *str, int *fd)
{
	int	j;

	j = 0;
	if (str[j] == '>')
	{
		if (*fd != 0)
			close (*fd);
		j++;
		if (str[j] == '>')
		{
			j++;
			*fd = open(&str[j], O_WRONLY | O_APPEND | O_CREAT, 0644);
			if (*fd < 0)
				perror(&str[j]);
		}
		else
			*fd = open(&str[j], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (*fd < 0)
			perror(&str[j]);
	}
}

static int	check_restdout(t_list *redir)
{
	int		fd;
	char	*str;

	fd = 0;
	while (fd >= 0 && redir)
	{
		str = (char *)redir->content;
		check_doublestdout(str, &fd);
		redir = redir->next;
	}
	return (fd);
}

void	redirect_streams(t_cmd *out, t_list *redir)
{
	int	fd_in;
	int	fd_out;

	fd_in = check_restdin(redir);
	fd_out = check_restdout(redir);
	if (fd_in <= -1)
	{
		out->infile = -1;
	}
	else if (fd_in > 0)
	{
		if (out->infile > 0)
			close(out->infile);
		out->infile = fd_in;
	}
	if (fd_out <= -1)
		out->outfile = -1;
	else if (fd_out > 0)
		out->outfile = fd_out;
	else if (fd_out == 0)
		out->outfile = 1;
}
