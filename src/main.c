#include "minishell.h"

void	new_line(int sig)
{
	(void)sig;
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
	if (!is_varname_ok(name))
	{
		free(name);
		return (0);
	}
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
	set_variable(&(global->local), name, content, DEFAULT_VAR);
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

void	refresh_status(t_command *global)
{
	char	*content;

	content = ft_itoa(global->last_status);
	set_variable(&(global->local), "?", content, DEFAULT_VAR);
	free(content);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_command	global;

	if (argc != 1)
		return (1);
	(void)argv;
	global.env = NULL;
	global.local = NULL;
	global.env = array_to_varlist(envp);
	global.last_status = 0;
	refresh_status(&global);
	signal(SIGINT, &new_line);
	while (1)
	{
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
			if (is_command(&global))
			{
				global.last_status = exec_cmd(global.cmds, global.env, &global);
				refresh_status(&global);
			}
			if( global.cmds)
				ft_lstfree(global.cmds, ft_array_free);
		}
		free(input);
	}
	rl_clear_history();
	return (0);
}
