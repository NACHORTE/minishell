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
	len = ft_strlen(parse.cmd[0]);
	write(2, parse.cmd[0], len);
	write(2, &"\n", 1);
}

void	execute(t_command parse, char **envp)
{
	if (!parse.cmd_path)
	{
		no_command(parse);
		exit(127);
	}
	execve(parse.cmd_path, parse.cmd, envp);
	exit(1);
}

void	new_line(int sig)
{
	printf("\n\033[36mminishell >> \033[0m");
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

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_command	parse;

	signal(SIGINT, &new_line);
	signal(SIGQUIT, SIG_IGN);
	parse.path = get_path(envp);
	while (1)
	{
		input = readline("\033[36mminishell >> \033[0m");
		if (input)
		{
			add_history(input);
			if (check_closed_quotes(input))
			{
				parse.cmd = ft_split_args(input, ' ');
				if (!parse.cmd[0])
					{}
				else if (!ft_strncmp(parse.cmd[0], "exit", 4))
				{
					free_double(parse.cmd);
					free(input);
					break;
				}
				else if (!ft_strncmp(parse.cmd[0], "cd", 2))
				{
					if (!parse.cmd[1] || !ft_strncmp(parse.cmd[1], "~", 1))
						chdir(getenv("HOME"));
					else
					{
						if (chdir(parse.cmd[1]) != 0)
							perror(parse.cmd[1]);
					}
				}
				else if (parse.cmd[0])
				{
					parse.cmd_path = get_cmd_path(parse.path, parse.cmd[0]);
					parse.child = fork();
					if (parse.child == 0)
						execute(parse, envp);
					waitpid(parse.child, NULL, 0);
					if (parse.cmd_path)
						free(parse.cmd_path);
					free_double(parse.cmd);
					free(input);
				}
				else
				{
					free_double(parse.cmd);
					free(input);
				}
			}
		}
	}
	rl_clear_history();
	free_double(parse.path);
	/*free_double(parsed);
	free(input);*/
	return (0);
}
