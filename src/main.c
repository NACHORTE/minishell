#include "minishell.h"

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
		if ((quotes % 2 == 0) && (input[i] == '\\' || input[i] == ';'))
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

void	sig_here(int sig)
{
	write(1, "\n", 1);
	exit (1);
}

int	here_doc(char *str)
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

int	check_restdin(char **input)
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
				fd = here_doc(&input[i][j + 1]);
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

int	check_restdout(char **input)
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

/*char	**parse_cmd(char **input)
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
}*/

/*void	cmd_pwd()
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
	char	*dst;

	if (global->cmd_parsed[1] != 0 && global->cmd_parsed[2] != 0)
	{
		printf("cd: too many arguments\n");
		return ;
	}
	if (!get_variable(global->env, "OLDPWD", &dst))
	{
		printf("cd: OLDPWD not set\n");
		return ;
	}
	old = malloc(sizeof(char) * 7);
	curr = malloc(sizeof(char) * 4);
	ft_strlcpy(old, "OLDPWD", 7);
	ft_strlcpy(curr, "PWD", 4);
	old_pwd = getcwd(NULL, 0);
	if (!global->cmd_parsed[1] || !ft_strcmp(global->cmd_parsed[1], "~"))
	{
		chdir(getenv("HOME"));
		curr_pwd = getcwd(NULL, 0);
	}
	else if (!ft_strcmp(global->cmd_parsed[1], "-"))    //COMPROBAR QUE HAY OLDPWD
	{
		chdir(dst);
		curr_pwd = getcwd(NULL, 0);
	}
	else
	{
		if (chdir(global->cmd_parsed[1]) != 0)
		{
			perror(global->cmd_parsed[1]);
			free(old);
			free(curr);
			free(old_pwd);
			return ;
		}
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
	if (!ft_strcmp(global->cmd_parsed[1], "-n"))
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
		get_variable(global->local, name, &content);
		content = ft_strdup(content);
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
	if (!ft_strcmp(global->cmd_parsed[0], "pwd"))
		cmd_pwd();
	else if (!ft_strcmp(global->cmd_parsed[0], "cd"))
		cmd_cd(global);
	else if (!ft_strcmp(global->cmd_parsed[0], "env"))
		cmd_env(global);
	else if (!ft_strcmp(global->cmd_parsed[0], "echo"))
		cmd_echo(global);
	else if (!ft_strcmp(global->cmd_parsed[0], "unset"))
		cmd_unset(global);
	else if (!ft_strcmp(global->cmd_parsed[0], "exit"))
		cmd_exit();
	else if (!ft_strcmp(global->cmd_parsed[0], "export"))
		cmd_export(global);
	else
		return (0);
	return (1);
}*/

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

int	just_redi(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '<' && str[i] != '>')
			return (0);
		i++;
	}
	return (1);
}

int	check_redi(char **cmd)
{
	int	i;
	int	j;

	i = 0;
	while (cmd[i])
	{
		if (just_redi(cmd[i]))
		{
			if ((cmd[i][0] == '>' || cmd[i][0] == '<')  && cmd[i + 1])
			{
				if (cmd[i + 1][0] == '>' || cmd[i + 1][0] == '<')
				{
					printf("syntax error near unexpected token '%c'\n", cmd[i + 1][0]);
					return (0);
				}
			}
		}
		i++;
	}
	i = 0;
	while (cmd[i])
	{
		j = 0;
		while (cmd[i][j])
		{
			if (cmd[i][j] != '>' && cmd[i][j] != '<')
				break;
			j++;
			if (!cmd[i][j])
			{
				printf("syntax error near unexpected token `newline'\n");
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

int	is_assignation(char *cmd)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (cmd[i])
	{
		if (i != 0)
		{
			if (cmd[i] == '=')
				if(cmd[i + 1])
				{
					flag = 1;
					break;
				}
		}
		i++;
	}
	return (flag);
}

int	is_allasignation(char **cmds)
{
	int	i;

	i = 0;
	while(cmds[i])
	{
		if (!is_assignation(cmds[i]))
			return (0);
		i++;
	}
	return (1);
}

int	set_assignation(t_command *global, int pos)
{
	int	i;
	char	*name;
	char	*content;
	int	size;
	int j;

	i = 0;
	size = 0;
	j = 0;
	while (((char **)global->cmds->content)[pos][i] != '=')
	{
		size++;
		i++;
	}
	i++;
	j = i;
	name = malloc(sizeof(char) * (size + 1));
	if (!name)
		return (0);
	ft_strlcpy(name, ((char **)global->cmds->content)[pos], size + 1);
	size = 0;
	while (((char **)global->cmds->content)[pos][i])
	{
		size++;
		i++;
	}
	content = malloc(sizeof(char) * (size + 1));
	if (!content)
		return (0);
	ft_strlcpy(content, &((char **)global->cmds->content)[pos][j], size + 1);
	set_variable(&(global->local), name, content);
	return (1);
}

int	save_variables(t_command *global)
{
	int	pos;

	pos = 0;
	while (((char **)global->cmds->content)[pos])
	{
		if (!set_assignation(global, pos))
			printf("No memory for new variable.\n");
		pos++;
	}
	return (1);
}

int local_declare(t_command *global)
{
	if (ft_lstsize(global->cmds) == 1 && is_allasignation((char **)global->cmds->content))
	{
		save_variables(global);
		return (1);
	}
	return (0);
}

int	is_command(t_command *global)
{
	if (!global->cmds || !((char **)global->cmds->content)[0])
		return (0);
	else if (local_declare(global))
		return (0);
	else if (!redirect_ok(global->cmds))
		return (0);
	else
		return (1);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_command	global;
	int i;
	int	j;
	int fd_out;
	int	fd_in;
	global.env = NULL;
	global.local = NULL;
	global.path = get_path(envp);
	save_env(&global, envp);
	global.sout = dup(1);
	global.sin = dup(0);
	while (1)
	{
		signal(SIGINT, &new_line);
		//signal(SIGQUIT, SIG_IGN);
		input = readline("\033[36mminishell >> \033[0m");
		if (input == NULL)
		{
			printf("exit\n");
			exit(1);
		}
		add_history(input);
		if (check_closed_quotes(input))
		{
			global.cmds = parse(input, global.local, global.env);
 			/* t_list *aux = global.cmds;
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
			if (is_command(&global))
			{
				i = 0;
				j = 0;
				//signal(SIGQUIT, SIG_DFL);
				/*global.cmd_parsed = parse_cmd((char **)global.cmds->content);
				global.cmd_path = get_cmd_path(global.path, global.cmd_parsed[0]); //once we hace the command check access
				fd_in = check_restdin((char **)global.cmds->content);
				fd_out = check_restdout((char **)global.cmds->content);
				if (fd_in < 0)
				{
					if (fd_out > 0)
						dup2(global.sout, 1);
				}
				else if (global.cmd_parsed[0] && !check_builtin(&global, envp))
				{
					global.child = fork();
					if (global.child == 0)
					{
						if (fd_in > 0)
						{
							dup2(fd_in, 0);
						}
						signal(SIGINT, SIG_DFL);
						//check_restdin((char **)global.cmds->content);  //check if redirections are made (stdin)
						//check_restdout((char **)global.cmds->content); //check if redirections are made (stdout)
						execute(global, envp); //execute command
					}
					else
					{
						if (fd_in > 0)
							close(fd_in);
						if (fd_out > 0)
							dup2(global.sout, 1);
						signal(SIGQUIT, SIG_IGN);
						signal(SIGINT, SIG_IGN);
					}
					waitpid(global.child, NULL, 0);
				}
				else
				{
					if (fd_out > 0)
						dup2(global.sout, 1);
				}*/
				exec_cmd(global.cmds, global.env, &global);
				/*if (global.cmd_path)
					free(global.cmd_path);
				if (!global.cmd_parsed[0])
					free(global.cmd_parsed);
				else
					ft_array_free(global.cmd_parsed);*/
			}
			if( global.cmds)
				ft_lstfree(global.cmds, ft_array_free);
			/*else
			{
				ft_lstfree(global.cmds, ft_array_free);
			}*/
		}
		free(input);
	}
	rl_clear_history();
	ft_array_free(global.path);
	return (0);
}
