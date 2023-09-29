/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:29:18 by oscar             #+#    #+#             */
/*   Updated: 2023/09/29 13:42:02 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, X_OK) == 0)
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
	while (*paths)
	{
		tmp = ft_strjoin(*paths, "/");
		command = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(command, X_OK) == 0)
			return (command);
		free(command);
		paths++;
	}
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	return (NULL);
}

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

static int	save_cmds(char **input, char **parsed)
{
	int	i;
	int	args;

	i = 0;
	args = 0;
	while (input[i])
	{
		if (input[i][0] != '<' && input[i][0] != '>')
		{
			parsed[args] = ft_strdup(input[i]);
			if (!parsed[args])
			{
				printf("problema dup\n");
				ft_array_free(parsed);
				return (1);
			}
			args++;
		}
		i++;
	}
	parsed[args] = 0;
	return (0);
}

char	**parse_cmd(char **input)
{
	int		i;
	char	**parsed;
	int		args;

	i = 0;
	args = 0;
	while (input[i])
	{
		if (input[i][0] != '<' && input[i][0] != '>')
			args++;
		i++;
	}
	parsed = malloc(sizeof(char *) * (args + 1));
	if (!parsed)
		return (NULL);
	if (save_cmds(input, parsed))
		return (NULL);
	return (parsed);
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
