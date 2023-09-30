/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_streams.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 14:31:06 by orudek            #+#    #+#             */
/*   Updated: 2023/09/29 19:24:29 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

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

static int	check_restdin(char **input)
{
	int	i;
	int	j;
	int	fd;

	i = 0;
	fd = 0;
	while (fd >= 0 && input[i])
	{
		j = 0;
		if (input[i][j] == '<')
			check_doublestdin(&input[i][j + 1], &fd);
		i++;
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

static int	check_restdout(char **input)
{
	int	i;
	int	fd;

	i = 0;
	fd = 0;
	while (fd >= 0 && input[i])
	{
		check_doublestdout(input[i], &fd);
		i++;
	}
	if (fd > 0)
	{
		dup2(fd, 1);
		close(fd);
	}
	return (fd);
}

void	redirect_streams(int infile, int outfile, char **cmd)
{
	int	fd_in;
	int	fd_out;

	fd_in = check_restdin(cmd);
	fd_out = check_restdout(cmd);
	if (fd_in <= -1)
	{
		exit(errno);
	}
	else if (fd_in == 0)
	{
		dup2(infile, 0);
		if (infile > 1)
			close(infile);
	}
	else
	{
		dup2(fd_in, 0);
		close(fd_in);
	}
	if (fd_out <= -1)
		exit(errno);
	else if (fd_out == 0)
		dup2(outfile, 1);
}
