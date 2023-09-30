/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/30 18:35:55 by iortega-          #+#    #+#             */
/*   Updated: 2023/09/30 18:48:39 by iortega-         ###   ########.fr       */
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

static void	sig_here(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit (1);
}

static void	new_line(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	read_heredoc(int *redi, char *str)
{
	char	*input;
	int		old;

	old = dup(1);
	signal(SIGINT, &sig_here);
	while (1)
	{
		dup2(old, 1);
		input = readline("> ");
		if (!input)
			exit (1);
		if (!ft_strcmp(input, str))
		{
			free(input);
			exit (0);
		}
		dup2(redi[1], 1);
		printf("%s\n", input);
		free(input);
	}
}

int	here_doc(char *str)
{
	int		redi[2];
	pid_t	sin;
	int		stat;

	pipe(redi);
	signal(SIGINT, SIG_IGN);
	sin = fork();
	if (sin == 0)
	{
		close(redi[0]);
		read_heredoc(redi, str);
	}
	else
		waitpid(sin, &stat, 0);
	signal(SIGINT, &new_line);
	stat = WEXITSTATUS(stat);
	close(redi[1]);
	if (stat == 1)
		return (-1);
	return (redi[0]);
}
