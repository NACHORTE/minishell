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

void	no_command(t_command global)
{
	int	len;
	write(2, &"Command not found: ", 19);
	len = ft_strlen(global.cmd_parsed[0]);
	write(2, global.cmd_parsed[0], len);
	write(2, &"\n", 1);
}

void	execute(t_command global, char **envp)
{
	if (!global.cmd_path)
	{
		no_command(global);
		exit(127);
	}
	execve(global.cmd_path, global.cmd_parsed, envp);
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
			if (input[i][j] == '>')
			{
				j++;
				fd = open(&input[i][j], O_WRONLY | O_APPEND, 0666);
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

int	cmd_cd(t_command *global)   //COMPROBAR MALLOCS
{
	char	*old;
	char	*curr;
	char	*old_pwd;
	char	*curr_pwd;

	if (global->cmd_parsed[1] != 0 && global->cmd_parsed[2] != 0)
	{
		printf("cd: too many arguments\n");
		return (0);
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
	return (0);
}

int	cmd_env(t_command *global)
{
	t_list	*aux;

	aux = global->env;
	while (aux)
	{
		printf("%s=%s\n", ((t_var *)aux->content)->name, ((t_var *)aux->content)->content);
		aux = aux->next;
	}
	return (1);
}

int cmd_echo(t_command *global)
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
	return (1);
}

int	cmd_unset(t_command *global)
{
	int	i;

	i = 1;
	while (global->cmd_parsed[i])
	{
		unset_variable(&(global->env), global->cmd_parsed[i]);
		i++;
	}
	return (1);
}

int	check_builtin(t_command *global, char **envp)
{
	if (!ft_strncmp(global->cmd_parsed[0], "pwd", 3))
	{
		cmd_pwd();
		return (1);
	}
	else if (!ft_strncmp(global->cmd_parsed[0], "cd", 2))
	{
		cmd_cd(global);
		return (1);
	}
	else if (!ft_strncmp(global->cmd_parsed[0], "env", 3))
	{
		cmd_env(global);
		return (1);
	}
	else if (!ft_strncmp(global->cmd_parsed[0], "echo", 4))
	{
		cmd_echo(global);
		return (1);
	}
	else if (!ft_strncmp(global->cmd_parsed[0], "unset", 5))
	{
		cmd_unset(global);
		return (1);
	}
	return (0);
}

int save_env(t_command *global, char **envp)
{
	int	len;
	int	i;
	int j;
	int aux;
	char	*name;
	char	*content;

	i = 0;
	while (envp[i])
	{
		j = 0;
		len = 0;
		while (envp[i][j] != '=')
		{
			len++;
			j++;
		}
		name = malloc(sizeof(char) * (len + 1));
		ft_strlcpy(name, envp[i], len + 1);
		j++;
		aux = j;
		len = 0;
		while (envp[i][j] != '\0')
		{
			len++;
			j++;
		}
		content = malloc(sizeof(char) * (len + 1));
		ft_strlcpy(content, &envp[i][aux], len + 1);
		set_variable(&(global->env), name, content);
		i++;
	}
	return (0);
}

int	check_redi(char **cmd)
{
	int	i;
	int	j;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i][0] == '>' && cmd[i + 1])
		{
			if (cmd[i + 1][0] == '>')
			{
				printf("syntax error near unexpected token '>'\n");
				return (0);
			}
		}
		i++;
	}
	return (1);
}

int redirect_ok(t_list *cmds)
{
	t_list	*aux;
	
	aux = cmds;
	while (aux)
	{
		if (!check_redi((char **)aux->content))
			return (0);
		aux = aux->next;
	}
	return (1);
}

int local_declare(t_command *global)
{
	int	i;
	char	*name;
	char	*content;
	int	size;
	int j;

	i = 0;
	size = 0;
	if (ft_lstsize(global->cmds) == 1)
	{
		if (((char **)global->cmds->content)[1] == 0)
		{
			while (((char **)global->cmds->content)[0][i] != '=')
			{
				if (((char **)global->cmds->content)[0][i] == 0)
					return (0);
				size++;
				i++;
			}
			i++;
			j = i;
			name = malloc(sizeof(char) * (size + 1));
			ft_strlcpy(name, ((char **)global->cmds->content)[0], size + 1);
			size = 0;
			while (((char **)global->cmds->content)[0][i])
			{
				size++;
				i++;
			}
			content = malloc(sizeof(char) * (size + 1));
			ft_strlcpy(content, &((char **)global->cmds->content)[0][j], size + 1);
			set_variable(&(global->local), name, content);
			return (1);
		}
	}
	return (0);
}

int	main(int argc, char **argv, char **envp) //hola
{
	char	*input;
	t_command	global;
	int i;
	int	j;
	int fd_out;
	//signal(SIGINT, &new_line);
	//signal(SIGINT, SIG_IGN);
	global.env = NULL;
	global.local = NULL;
	global.path = get_path(envp);
	save_env(&global, envp);
	global.sout = dup(1);
	while (1)
	{
		signal(SIGINT, &new_line);
		signal(SIGQUIT, SIG_IGN);
		//signal(SIGINT, SIG_DFL);
		input = readline("\033[36mminishell >> \033[0m");
		//signal(SIGINT, &new_line_father);
		//input = readline("\033[36mminishell >> \033[0m");
		if (iw)
		{
			printf("exit\n");
			exit(1);
		}
		if (ft_strlen(input) > 0)
		{
			add_history(input);
			if (check_closed_quotes(input))
			{
				global.cmds = parse(input, global.local, global.env);
/* 				t_list *aux = global.cmds;
				int j = 0;
				while (aux)
				{
					printf("List %d\n",j++);
					int i = 0;
					for (i = 0; ((char **)aux->content)[i]; i++)
						printf("\tstr[%d]=%s$\n",i,((char **)aux->content)[i]);
					printf("\tstr[%d]=%s$\n",i,((char **)aux->content)[i]);
					aux = aux->next;
				} */
				if (!global.cmds || !((char **)global.cmds->content)[0])
					{
						if( global.cmds)
							ft_lstfree(global.cmds, ft_array_free);
						free(input);
					}
				else if (local_declare(&global))
				{
					ft_lstfree(global.cmds, ft_array_free);
					free(input);
				}
				else if (!redirect_ok(global.cmds))
				{
					ft_lstfree(global.cmds, ft_array_free);
					free(input);
				}
				else if (!ft_strncmp(((char **)global.cmds->content)[0], "exit", 4))
				{
					//free_double(global.cmd);
					ft_lstfree(global.cmds, ft_array_free);
					free(input);
					printf("exit\n");
					exit(1);
				}
				else if (((char **)global.cmds->content)[0])
				{
					i = 0;
					j = 0;
					signal(SIGQUIT, SIG_DFL);
					global.cmd_parsed = parse_cmd((char **)global.cmds->content);
					global.cmd_path = get_cmd_path(global.path, global.cmd_parsed[0]); //once we hace the command check access
					fd_out = check_restdout((char **)global.cmds->content);
					if (!check_builtin(&global, envp))
					{
						if (fd_out > 0)
						{
							dup2(global.sout, 1);
						}
						global.child = fork();
						if (global.child == 0)
						{
							signal(SIGINT, SIG_DFL);
							check_restdin((char **)global.cmds->content);  //check if redirections are made (stdin)
							check_restdout((char **)global.cmds->content); //check if redirections are made (stdout)
							execute(global, envp); //execute command
							//exit(0);
						}
						else
							signal(SIGINT, SIG_IGN);
						waitpid(global.child, NULL, 0);
					}
					else if (fd_out > 0)
					{
						dup2(global.sout, 1);
					}
					if (global.cmd_path)
						free(global.cmd_path);
					//free_double(global.cmd);
					ft_lstfree(global.cmds, ft_array_free);
					free_double(global.cmd_parsed);
					free(input);
				}
				else
				{
					//free_double(global.cmd);
					ft_lstfree(global.cmds, ft_array_free);
					free(input);
				}
			}
		}
		else
			free(input);
	}
	rl_clear_history();
	free_double(global.path);
	/*free_double(parsed);
	free(input);*/
	return (0);
}
