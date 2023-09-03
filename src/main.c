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

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_command	parse;

	parse.path = get_path(envp);
	while (1)
	{
		input = readline("\033[36mminishell >> \033[0m");
		parse.cmd = ft_split(input, ' ');
		if (parse.cmd[0])
		{
			add_history(input);
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
	free_double(parse.path);
	/*free_double(parsed);
	free(input);*/
	return (0);
}
