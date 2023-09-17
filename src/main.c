#include "minishell.h"

void	free_double(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
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

char	*get_cmd_path(char **paths, char *cmd)
{
	char	*tmp;
	char	*command;
	int		abs;

	abs = 0;
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

char	**get_path(char **envp)
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

void	no_command(t_command parse)
{
	int	len;
	write(2, &"Command not found: ", 19);
	len = ft_strlen(parse.cmd_parsed[0]);
	write(2, parse.cmd_parsed[0], len);
	write(2, &"\n", 1);
}

void	execute(t_command parse, char **envp)
{
	if (!parse.cmd_path)
	{
		no_command(parse);
		exit(127);
	}
	execve(parse.cmd_path, parse.cmd_parsed, envp);
	exit(1);
}

void	new_line(int sig)
{
	//printf("\n\033[36mminishell >> \033[0m");
	write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
	//exit(1);
}

int check_closed_quotes(char *input)
{
	int	i;
	int	quotes;

	i = 0;
	quotes = 0;
	while (input[i])
	{
		if (input[i] == '"')
			quotes++;
		if (input[i] == '\\' || input[i] == ';')
		{
			printf("Invalid character: \"%c\"\n", input[i]);
			return (0);
		}
		i++;
	}
	if (quotes % 2 == 0)
		return (1);
	else
	{
		printf("Please close quotes.\n");
		return (0);
	}
}

void	check_restdin(char **input)
{
	int	i;
	int	j;
	int	flag;
	int	fd;

	i = 0;
	flag = 0;
	/*while (input[i])
	{
		printf("%s\n", input[i]);
		i++;
	}
	i = 0;*/
	while (input[i])   //if we have < " " file we get flag 1 and check file, else if we have <file we check file without "<"
	{
		j = 0;
		if (ft_strlen(input[i]) == 1 && input[i][j] == '<')
		{
			i++;
			flag = 1;
			break;
		}
		else if (input[i][j] == '<')
		{
			j++;
			flag = 1;
			break;
		}
		i++;
	}
	if (flag)
	{
		fd = open(&input[i][j], O_RDONLY);
		if (fd < 0)
		{
			perror(&input[i][j]);
			exit(1);
		}
		dup2(fd, 0);
		close(fd);
	}
}

int	check_restdout(char **input)
{
	int	i;
	int	j;
	int	flag;
	int	fd;
	char	*redi;

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
			if (fd != 0)
				close (fd);
			i++;
			flag = 1;
			redi = &input[i][j];
			fd = open(&input[i][j], O_WRONLY | O_TRUNC | O_CREAT, 0666);
		if (fd < 0)
		{
			perror(&input[i][j]);
			return (-1);
		}
			//break;
		}
		else if (input[i][j] == '>')
		{
			if (fd != 0)
				close (fd);
			j++;
			flag = 1;
			redi = &input[i][j];
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
		/*fd = open(&input[i][j], O_WRONLY | O_TRUNC | O_CREAT, 0666);
		if (fd < 0)
		{
			perror(&input[i][j]);
			exit(1);
		}*/
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
	/*while(input[i])
	{
		printf("%s\n", input[i]);
		i++;
	}
	i = 0;*/
	args = 0;
	while(input[i])
	{
		if (input[i][0] != '<' && input[i][0] != '>')
			args++;
		/*else
			i++;
		if (!input[i])
			break;*/
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
		else if (ft_strlen(input[i]) == 1)
			i++;
		i++;
	}
	parsed[args] = 0;
	/*i = 0;
	while(parsed[i])
	{
		printf("%s\n", parsed[i]);
		i++;
	}*/
	return (parsed);
}

int	cmd_pwd()
{
	char	*dir;

	dir = getcwd(NULL, 0);
	if (dir == NULL)
	{
		printf("ERROR: PWD\n");
		return (0);
	}
	printf("%s\n", dir);
	free (dir);
	return (1);
}

int	cmd_cd(t_command *parse)   //COMPROBAR MALLOCS
{
	t_var	*old;
	t_var	*curr;

	if (parse->cmd[1] != 0 && parse->cmd[2] != 0)
	{
		printf("cd: too many arguments\n");
		return (0);
	}
	old = malloc(sizeof(t_var));
	curr = malloc(sizeof(t_var));
	old->name = malloc(sizeof(char) * 7);
	curr->name = malloc(sizeof(char) * 4);
	ft_strlcpy(old->name, "OLDPWD", 7);
	ft_strlcpy(curr->name, "PWD", 4);
	old->content = getcwd(NULL, 0);
	if (!parse->cmd[1] || !ft_strncmp(parse->cmd[1], "~", 1))
	{
		chdir(getenv("HOME"));
		curr->content = getcwd(NULL, 0);
	}
	else if (!ft_strncmp(parse->cmd[1], "-", 1))    //COMPROBAR QUE HAY OLDPWD
	{
		chdir(get_variable(parse->env, "OLDPWD"));
		curr->content = getcwd(NULL, 0);
	}
	else
	{
		if (chdir(parse->cmd[1]) != 0)
			perror(parse->cmd[1]);
		else
			curr->content = getcwd(NULL, 0);
	}
	set_variable(&(parse->env), old);
	set_variable(&(parse->env), curr);
	return (0);
}

int	cmd_env(t_command *parse)
{
	t_list	*aux;

	aux = parse->env;
	while (aux)
	{
		printf("%s=%s\n", ((t_var *)aux->content)->name, ((t_var *)aux->content)->content);
		aux = aux->next;
	}
	return (1);
}

int cmd_echo(t_command *parse)
{
	int	i;
	int	flag;

	i = 1;
	flag = 0;
	if (!ft_strncmp(parse->cmd_parsed[1], "-n", 2))
	{
		flag = 1;
		i++;
	}
	while(parse->cmd_parsed[i])
	{
		if (parse->cmd_parsed[i + 1] == 0)
		{
			printf("%s", parse->cmd_parsed[i]);
			if (flag == 0)
				printf("\n");
		}
		else
			printf("%s ",parse->cmd_parsed[i]);
		i++;
	}
	return (1);
}

int	check_builtin(t_command *parse, char **envp)
{
	if (!ft_strncmp(parse->cmd_parsed[0], "pwd", 3))
	{
		cmd_pwd();
		return (1);
	}
	else if (!ft_strncmp(parse->cmd_parsed[0], "cd", 2))
	{
		cmd_cd(parse);
		return (1);
	}
	else if (!ft_strncmp(parse->cmd_parsed[0], "env", 3))
	{
		cmd_env(parse);
		return (1);
	}
	else if (!ft_strncmp(parse->cmd_parsed[0], "echo", 4))
	{
		cmd_echo(parse);
		return (1);
	}
	return (0);
}

int save_env(t_command *parse, char **envp)
{
	t_var *tmp;
	int	len;
	int	i;
	int j;
	int aux;

	i = 0;
	while (envp[i])
	{
		tmp = malloc(sizeof(t_var));
		j = 0;
		len = 0;
		while (envp[i][j] != '=')
		{
			len++;
			j++;
		}
		tmp->name = malloc(sizeof(char) * (len + 1));
		ft_strlcpy(tmp->name, envp[i], len + 1);
		j++;
		aux = j;
		len = 0;
		while (envp[i][j] != '\0')
		{
			len++;
			j++;
		}
		tmp->content = malloc(sizeof(char) * (len + 1));
		ft_strlcpy(tmp->content, &envp[i][aux], len + 1);
		set_variable(&(parse->env), tmp);
		i++;
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_command	parse;
	int i;
	int	j;
	int fd_out;

	//signal(SIGINT, &new_line);
	//signal(SIGINT, SIG_IGN);
	//signal(SIGQUIT, SIG_IGN);
	parse.env = NULL;
	parse.path = get_path(envp);
	save_env(&parse, envp);
	parse.sout = dup(1);
	while (1)
	{
		signal(SIGINT, &new_line);
		//signal(SIGINT, SIG_DFL);
		input = readline("\033[36mminishell >> \033[0m");
		//signal(SIGINT, &new_line_father);
		//input = readline("\033[36mminishell >> \033[0m");
		if (ft_strlen(input) > 0)
		{
			add_history(input);
			if (check_closed_quotes(input))
			{
				parse.cmd = split_args(input, ' ');
				if (!parse.cmd[0])
					{
						free(parse.cmd);
						free(input);
					}
				else if (!ft_strncmp(parse.cmd[0], "exit", 4))
				{
					free_double(parse.cmd);
					free(input);
					exit(1);
				}
				/*else if (!ft_strncmp(parse.cmd[0], "cd", 2))
				{
					if (!parse.cmd[1] || !ft_strncmp(parse.cmd[1], "~", 1))
						chdir(getenv("HOME"));
					else
					{
						if (chdir(parse.cmd[1]) != 0)
							perror(parse.cmd[1]);
					}
					free_double(parse.cmd);
					free(input);
				}*/
				else if (parse.cmd[0])
				{
					i = 0;
					j = 0;
					parse.cmd_parsed = parse_cmd(parse.cmd);
					parse.cmd_path = get_cmd_path(parse.path, parse.cmd_parsed[0]); //once we hace the command check access
					fd_out = check_restdout(parse.cmd);
					if (!check_builtin(&parse, envp))
					{
						if (fd_out > 0)
						{
							dup2(parse.sout, 1);
						}
						parse.child = fork();
						if (parse.child == 0)
						{
							signal(SIGINT, SIG_DFL);
							check_restdin(parse.cmd);  //check if redirections are made (stdin)
							check_restdout(parse.cmd); //check if redirections are made (stdout)
							execute(parse, envp); //execute command
							//exit(0);
						}
						else
							signal(SIGINT, SIG_IGN);
						waitpid(parse.child, NULL, 0);
					}
					else if (fd_out > 0)
					{
						dup2(parse.sout, 1);
					}
					if (parse.cmd_path)
						free(parse.cmd_path);
					free_double(parse.cmd);
					free_double(parse.cmd_parsed);
					free(input);
				}
				else
				{
					free_double(parse.cmd);
					free(input);
				}
			}
		}
		else
			free(input);
	}
	rl_clear_history();
	free_double(parse.path);
	/*free_double(parsed);
	free(input);*/
	return (0);
}
