/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:29:18 by oscar             #+#    #+#             */
/*   Updated: 2023/10/06 12:57:50 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_cmd.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/errno.h>

/*  child:
        Executes a command that can have redirections.
        If the command doesn't have a command redirection for a stream, uses
        the corresponding one from the function parameters.
    Parameters:
        infile: File descriptor of the file from which the command will read
            the data
        outfile: File descriptor of the file to which the command will write
			the data
		cmd: Array of strings that each contains an argument of the command
			(eg. [cat] [>outfile] [Makefile])
		env: Array of strings containing all the environment variables (not
			the local ones).
	Return:
		NOTHING! it must exit the program at the end of this function
*/
static char	**get_path(char **envp)
{
	char	**path;

	while (*envp)
	{
		if (!ft_strncmp("PATH=", *envp, 5))
			break ;
		envp++;
	}
	if (!*envp)
		return (NULL);
	path = ft_split(*envp + 5, ':');
	return (path);
}

static char	*absolute_route(char *cmd, int *abs)
{
	if (*cmd == '/' || !ft_strncmp(cmd, "./", 2) || !ft_strncmp(cmd, "../", 3))
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	*abs = 1;
	return (NULL);
}

static void	no_command(char **cmd_parsed)
{
	int	len;

	if (!*cmd_parsed)
		exit (0);
	write(2, &"Command not found: ", 19);
	len = ft_strlen(cmd_parsed[0]);
	if (**cmd_parsed)
		write(2, cmd_parsed[0], len);
	else
		write(2, "\'\'", 2);
	write(2, &"\n", 1);
	exit(127);
}

static char	*get_cmd_path(char **paths, char *cmd)
{
	char	*tmp;
	char	*command;
	int		abs;

	abs = 0;
	if (!cmd || !*cmd)
		return (NULL);
	command = absolute_route(cmd, &abs);
	if (!abs)
		return (command);
	while (paths && *paths)
	{
		tmp = ft_strjoin(*paths, "/");
		command = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(command, F_OK) == 0)
			return (command);
		free(command);
		paths++;
	}
	if (access(cmd, F_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}
/*
int	check_permission(char **cmd)
{
	int	i;
	int	j;
	int fd;

	i = 0;
	fd = 0;
	while (cmd[i])
	{
		j = 0;
		if (cmd[i][j] == '<')
		{
			j++;
			if (cmd[i][j] != '<')
				fd = open(&cmd[i][j], O_RDONLY);
		}
		else if (cmd[i][j] == '>') //check
		{
			j++;
			if (cmd[i][j] == '>')
				fd = open(&cmd[i][++j], O_WRONLY | O_APPEND | O_CREAT, 0644);
			else
				fd = open(&cmd[i][j], O_WRONLY | O_CREAT, 0644);
		}
		if (fd < 0)
		{
			perror(&cmd[i][j]);
			return (1);
		}
		if (fd > 0)
			close(fd);
		i++;
	}
	return (0);
}*/

void	child(int infile, int outfile, char **cmd, t_list **varlist)
{
	char	*cmd_path;
	char	**env;
	char	**path;

	signal(SIGINT, SIG_DFL);
	env = varlist_to_array(*varlist, ENV_VAR);
	path = get_path(env);
	if (infile > 1)
	{
		dup2(infile, 0);
		close(infile);
	}
	if (outfile > 1)
	{
		dup2(outfile, 1);
		close(outfile);
	}
	if (is_builtin(cmd[0]))
		exit(exec_builtin(cmd, varlist));
	cmd_path = get_cmd_path(path, cmd[0]);
	if (!cmd_path)
		no_command(cmd);
	execve(cmd_path, cmd, env);
	perror(cmd[0]);
	exit(errno);
}
