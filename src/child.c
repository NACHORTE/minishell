/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:29:18 by oscar             #+#    #+#             */
/*   Updated: 2023/09/26 20:43:04 by iortega-         ###   ########.fr       */
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

#include "minishell.h"

static void	new_line(int sig)
{
	//printf("\n\033[36mminishell >> \033[0m");
	write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
	//exit(1);
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

static void	sig_here(int sig)
{
	write(1, "\n", 1);
	exit (1);
}

static int	here_doc(char *str)
{
	int	redi[2];
	char	*input;
	pid_t	sin;
	int	old;
	int	stat;

	pipe(redi);
	old = dup(1);
	signal(SIGINT, SIG_IGN);
	sin = fork();
	if (sin == 0)
	{
		signal(SIGINT, &sig_here);
		close(redi[0]);
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
	else
		waitpid(sin, &stat, 0);
	signal(SIGINT, &new_line);
	stat = WEXITSTATUS(stat);
	close(redi[1]);
	if (stat == 1)
		return (-1);
	return (redi[0]);
	/*dup2(redi[0], 0);
	close(redi[0]);
	return (1);*/
}

static int	check_restdin(char **input)
{
	int	i;
	int	j;
	int	flag;
	int	fd;

	i = 0;
	flag = 0;
	fd = 0;
	/*while (input[i])
	{
		printf("%s\n", input[i]);
		i++;
	}
	i = 0;*/
	while (input[i])   //if we have < " " file we get flag 1 and check file, else if we have <file we check file without "<"
	{
		j = 0;
		if (input[i][j] == '<')
		{
			if (fd != 0)
				close (fd);
			j++;
			flag = 1;
			if (input[i][j] == '<')
			{
				//fd = here_doc(&input[i][j + 1]);
				if (fd != 0)
					close (fd);
				fd = 0;
			}
			else
			{
				fd = open(&input[i][j], O_RDONLY);
				if (fd < 0)
				{
					perror(&input[i][j]);
					return (-1);
				}
			}
		}
		i++;
	}
	/*if (flag)
	{
		dup2(fd, 0);
		close(fd);
	}*/
	return (fd);
}

static int	check_restdout(char **input)
{
	int	i;
	int	j;
	int	flag;
	int	fd;

	i = 0;
	flag = 0;
	fd = 0;
	/*while (input[i])
	{
		printf("%s\n", input[i]);
		i++;
	}
	i=0;*/
	while (input[i])   //if we have > " " file we get flag 1 and check file, else if we have >file we check file without ">"
	{
		j = 0;
		if (ft_strlen(input[i]) == 1 && input[i][j] == '>')
		{
			break;
		}
		else if (input[i][j] == '>')
		{
			if (fd != 0)
				close (fd);
			j++;
			flag = 1;
			if (input[i][j] == '>')
			{
				j++;
				fd = open(&input[i][j], O_WRONLY | O_APPEND | O_CREAT, 0666);
			}
			else
				fd = open(&input[i][j], O_WRONLY | O_TRUNC | O_CREAT, 0666);
		if (fd < 0)
		{
			perror(&input[i][j]);
			return (-1);
		}
			//break;
		}
		i++;
	}
	if (flag)
	{
		dup2(fd, 1);
		close(fd);
	}
	return (fd);
}

char	**parse_cmd(char **input)
{
	int	i;
	char	**parsed;
	int	args;

	i = 0;
	args = 0;
	while(input[i])
	{
		if (input[i][0] != '<' && input[i][0] != '>')
			args++;
		i++;
	}
	parsed = malloc(sizeof(char *) * (args + 1));
	if (!parsed)
		return (NULL);
	i = 0;
	args = 0;
	while (input[i])
	{
		if (input[i][0] != '<' && input[i][0] != '>')
		{
			parsed[args] = ft_strdup(input[i]);   //parchear malloc dup
			if (!parsed[args])
			{
				printf("problema dup\n");
				ft_array_free(parsed);
				return (NULL);
			}
			args++;
		}
		i++;
	}
	parsed[args] = 0;
	return (parsed);
}

void	redirect_streams(int infile, int outfile, char **cmd)
{
	int	fd_in;

	fd_in = check_restdin(cmd);
	if (fd_in <= 0)
		dup2(infile, 0);
	else
	{
		dup2(fd_in, 0);
		close(fd_in);
	}
	if (check_restdout(cmd) <= 0)
		dup2(outfile, 1);
}

void    child(int infile, int outfile, char **cmd, t_command *global)
{
	char	**cmd_parsed;
	char	*cmd_path;
	char	**env;
	//makes the needed dup2, creates the files if there are multiple output redirections
	// and opens the correct file.
	env = varlist_to_array(global->env, 1);
    redirect_streams(infile, outfile, cmd);
    //removes the redirections from the command returning a new char **array
    cmd_parsed = parse_cmd(cmd);
    //gets the full path of the command
	if (is_builtin(cmd_parsed[0]))
		exit(exec_builtin(cmd_parsed, &global->local, &global->env));
    cmd_path = get_cmd_path(global->path, cmd_parsed[0]);
	if (!cmd_path)
	{
		no_command(cmd_parsed);
		exit(127);
	}
    execve(cmd_path, cmd_parsed, env);
	perror(cmd_parsed[0]);
	exit(errno); //NOTE maybe just exit 1 is OK
}