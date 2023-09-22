/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:29:18 by oscar             #+#    #+#             */
/*   Updated: 2023/09/22 22:14:46 by orudek           ###   ########.fr       */
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

void	cmd_pwd()
{
	char	*dir;

	dir = getcwd(NULL, 0);
	if (dir == NULL)
	{
		printf("ERROR: PWD\n");
		return ;
	}
	printf("%s\n", dir);
	free (dir);
}

void	cmd_cd(t_command *global)   //COMPROBAR MALLOCS
{
	char	*old;
	char	*curr;
	char	*old_pwd;
	char	*curr_pwd;

	if (global->cmd_parsed[1] != 0 && global->cmd_parsed[2] != 0)
	{
		printf("cd: too many arguments\n");
		return ;
	}
	old = malloc(sizeof(char) * 7);
	curr = malloc(sizeof(char) * 4);
	ft_strlcpy(old, "OLDPWD", 7);
	ft_strlcpy(curr, "PWD", 4);
	old_pwd = getcwd(NULL, 0);
	if (!global->cmd_parsed[1] || !ft_strncmp(global->cmd_parsed[1], "~", 1))
	{
		chdir(getenv("HOME"));
		curr_pwd = getcwd(NULL, 0);
	}
	else if (!ft_strncmp(global->cmd_parsed[1], "-", 1))    //COMPROBAR QUE HAY OLDPWD
	{
		chdir(get_variable(global->env, "OLDPWD"));
		curr_pwd = getcwd(NULL, 0);
	}
	else
	{
		if (chdir(global->cmd_parsed[1]) != 0)
			perror(global->cmd_parsed[1]);
		else
			curr_pwd = getcwd(NULL, 0);
	}
	set_variable(&(global->env), old, old_pwd);
	set_variable(&(global->env), curr, curr_pwd);
}

void	cmd_env(t_command *global)
{
	t_list	*aux;

	aux = global->env;
	while (aux)
	{
		printf("%s=%s\n", ((t_var *)aux->content)->name, ((t_var *)aux->content)->content);
		aux = aux->next;
	}
}

void cmd_echo(t_command *global)
{
	int	i;
	int	flag;

	i = 1;
	flag = 0;
	if (!ft_strncmp(global->cmd_parsed[1], "-n", 2))
	{
		flag = 1;
		i++;
	}
	while(global->cmd_parsed[i])
	{
		if (global->cmd_parsed[i + 1] == 0)
		{
			printf("%s", global->cmd_parsed[i]);
			if (flag == 0)
				printf("\n");
		}
		else
			printf("%s ",global->cmd_parsed[i]);
		i++;
	}
}

void	cmd_unset(t_command *global)
{
	int	i;

	i = 1;
	while (global->cmd_parsed[i])
	{
		unset_variable(&(global->env), global->cmd_parsed[i]);
		i++;
	}
}

void	cmd_exit()
{
	printf("exit\n");
	exit(1);
}

void	cmd_export(t_command *global)
{
	char	*name;
	char	*content;
	int	i;

	i = 1;
	while (((char **)global->cmds->content)[i])
	{
		name = ft_strdup(((char **)global->cmds->content)[i]);
		content = ft_strdup(get_variable(global->local, name));
		if (!content)
		{
			free(name);
		}
		else
		{
			set_variable(&(global->env), name, content);
			unset_variable(&(global->local), name);
		}
		i++;
	}
}

int	check_builtin(t_command *global, char **envp)
{
	if (!ft_strncmp(global->cmd_parsed[0], "pwd", 3))
		cmd_pwd();
	else if (!ft_strncmp(global->cmd_parsed[0], "cd", 2))
		cmd_cd(global);
	else if (!ft_strncmp(global->cmd_parsed[0], "env", 3))
		cmd_env(global);
	else if (!ft_strncmp(global->cmd_parsed[0], "echo", 4))
		cmd_echo(global);
	else if (!ft_strncmp(global->cmd_parsed[0], "unset", 5))
		cmd_unset(global);
	else if (!ft_strncmp(global->cmd_parsed[0], "exit", 4))
		cmd_exit();
	else if (!ft_strncmp(global->cmd_parsed[0], "export", 6))
		cmd_export(global);
	else
		return (0);
	return (1);
}

int	check_restdin(char **input)
{
	int	i;
	int	j;
	int	flag;
	int	fd;

	i = 0;
	flag = 0;
	while (input[i])   //if we have < " " file we get flag 1 and check file, else if we have <file we check file without "<"
	{
		j = 0;
		if (input[i][j] == '<')
		{
			if (fd != 0)
				close (fd);
			j++;
			fd = open(&input[i][j], O_RDONLY);
			if (fd < 0)
			{
				perror(&input[i][j]);
				exit(1);
			}
			flag = 1;
		}
		i++;
	}
	if (flag)
	{
		dup2(fd, 0);
		close(fd);
		return (1);
	}
	return (0);
}

int	check_restdout(char **input)
{
	int	i;
	int	j;
	int	flag;
	int	fd;

	i = 0;
	flag = 0;
	fd = 0;
	while (input[i])   //if we have > " " file we get flag 1 and check file, else if we have >file we check file without ">"
	{
		j = 0;
		if (input[i][j] == '>')
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
				free_double(parsed);
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
	if (check_restdin(cmd) <= 0)
		dup2(infile, 0);
	if (check_restdout(cmd) <= 0)
		dup2(outfile, 1);
}

void    child(int infile, int outfile, char **cmd, char **env)
{
	//makes the needed dup2, creates the files if there are multiple output redirections
	// and opens the correct file.
    redirect_streams(infile, outfile, cmd);
    //removes the redirections from the command returning a new char **array
    cmd_parsed = parse_cmd(cmd);
    //gets the full path of the command
    cmd_path = get_cmd_path();
	if (check_builtin())
		exit (1);
    excve(cmd_path, cmd_parsed, env);
	perror(cmd_parsed[0]);
	exit(errno); //NOTE maybe just exit 1 is OK
}