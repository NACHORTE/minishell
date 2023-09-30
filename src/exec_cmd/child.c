/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:29:18 by oscar             #+#    #+#             */
/*   Updated: 2023/09/30 19:39:02 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_cmd.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <readline/readline.h>
#include <readline/history.h>

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
	Todo:
		In a separate function
		[x] REDIR: Create all files from the redirections in cmd
		[x] REDIR: <<Here Doc
		[x] REDIR: >>append mode
		[x] REDIR: If no redirection in the cmd, redirect to infile/outfile
		[x]	CHECK_BUILTINS
		[ ] ACCESS: "./" "../" "/"
		[ ] ACCESS: [OPTIONAL] if no path, add ./ to cmd if it doesn't have it
		[ ] ACCESS: if cmd[i]="\0", don't crash and print "'': cmd not found"
		[ ] perror with cmd name after excve
		[ ] use nice names :)
		[ ] algo de signals nose
		[ ] norminette
*/

#include "exec_cmd.h"

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

	write(2, &"Command not found: ", 19);
	len = ft_strlen(cmd_parsed[0]);
	write(2, cmd_parsed[0], len);
	write(2, &"\n", 1);
}

static char	*get_cmd_path(char **paths, char *cmd)
{
	char	*tmp;
	char	*command;
	int		abs;

	abs = 0;
	if (!cmd)
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

void	child(int infile, int outfile, char **cmd, t_list **varlist)
{
	char	**cmd_parsed;
	char	*cmd_path;
	char	**env;
	char	**path;

	env = varlist_to_array(*varlist, ENV_VAR);
	path = get_path(env);
	redirect_streams(infile, outfile, cmd);
	cmd_parsed = parse_cmd(cmd);
	if (is_builtin(cmd_parsed[0]))
		exit(exec_builtin(cmd_parsed, varlist));
	cmd_path = get_cmd_path(path, cmd_parsed[0]);
	if (!cmd_path)
	{
		no_command(cmd_parsed);
		exit(127);
	}
	execve(cmd_path, cmd_parsed, env);
	perror(cmd_parsed[0]);
	exit(errno);
}
