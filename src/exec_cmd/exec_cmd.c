/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:56:41 by oscar             #+#    #+#             */
/*   Updated: 2023/09/30 14:39:15 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_cmd.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/wait.h>

#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

int	count_heredoc(t_list *cmds)
{
	int	i;
	int	count;

	count = 0;
	while (cmds)
	{
		i = 0;
		while (((char **)cmds->content)[i])
		{
			if (!ft_strncmp(((char **)cmds->content)[i], "<<", 2))
				count++;
			i++;
		}
		cmds = cmds->next;
	}
	return (count);
}

static void	sig_here(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit (1);
}

static void	new_line(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	read_heredoc(int *redi, char *str)
{
	char	*input;
	int		old;

	old = dup(1);
	signal(SIGINT, &sig_here);
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

static int	here_doc(char *str)
{
	int		redi[2];
	pid_t	sin;
	int		stat;

	pipe(redi);
	signal(SIGINT, SIG_IGN);
	sin = fork();
	if (sin == 0)
	{
		close(redi[0]);
		read_heredoc(redi, str);
	}
	else
		waitpid(sin, &stat, 0);
	signal(SIGINT, &new_line);
	stat = WEXITSTATUS(stat);
	close(redi[1]);
	if (stat == 1)
		return (-1);
	return (redi[0]);
}

static void	check_doublestdin(char *character, int *fd)
{
	if (*fd != 0)
		close (*fd);
	if (*character == '<')
	{
		*fd = here_doc(character + 1);
	}
	else
	{
		if (*fd != 0)
			close (*fd);
		*fd = 0;
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
		{
			check_doublestdin(&input[i][j + 1], &fd);
		}
		i++;
	}
	return (fd);
}

static void	error_heredoc(int **fds)
{
	int	i;

	i = 0;
	while ((*fds)[i] != -1)
	{
		close((*fds)[i]);
		i++;
	}
}

int	open_heredocs(t_list *cmds, int **n_cmd, int **fds)
{
	int	fd;
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cmds)
	{
		fd = 0;
		fd = check_restdin((char **)cmds->content);
		if (fd != 0)
		{
			(*n_cmd)[j] = i;
			(*fds)[j] = fd;
			j++;
		}
		if (fd == -1)
		{
			error_heredoc(fds);
			return (1);
		}
		i++;
		cmds = cmds->next;
	}
	return (0);
}

//[ ] check open files and leaks
int	exec_one_builtin(char **cmd, t_list **varlist, int *status)
{
	int		std_in;
	int		std_out;
	char	**cmd_parsed;

	cmd_parsed = parse_cmd(cmd);
	if (!is_builtin(cmd_parsed[0]))
	{
		ft_array_free(cmd_parsed);
		return (0);
	}
	std_in = dup(0);
	std_out = dup(1);
	redirect_streams(0, 1, cmd);
	*status = exec_builtin(cmd_parsed, varlist);
	ft_array_free(cmd_parsed);
	dup2(std_in, 0);
	dup2(std_out, 1);
	close(std_in);
	close(std_out);
	return (1);
}

/*  If the command given is a builtin, then no fork must be made and executes
    the function for builtins. If it is a normal command, creates a fork and
    calls the child process setting infile=stdin, outfile=stdout
    If the command contains redirections, they will be handled inside the child
*/
int	exec_one_cmd(char **cmd, t_list **varlist)
{
	int	status;
	int	fd;
	int	pid;

	fd = check_restdin(cmd);
	if (fd < 0)
		return (1);
	if (exec_one_builtin(cmd, varlist, &status))
		return (status);
	pid = fork();
	if (pid == -1)
	{
		printf("EXEC_ONE_CMD: fork fail\n");
		return (1);
	}
	if (pid == 0)
		child(fd, 1, cmd, varlist);
	if (fd > 0)
		close(fd);
	wait(&status);
	return (WEXITSTATUS(status));
}

int	manage_heredocs(t_multicmd *data, t_list *cmds)
{
	data->n_heredocs = count_heredoc(cmds);
	if (data->n_heredocs > 0)
	{
		data->n_cmd = malloc(sizeof(int) * data->n_heredocs);
		data->fds = malloc(sizeof(int) * data->n_heredocs);
		if (open_heredocs(cmds, &(data->n_cmd), &(data->fds)))
		{
			free(data->n_cmd);
			free(data->fds);
			return (1);
		}
	}
	return (0);
}

int	first_cmd(t_multicmd *data, t_list *cmds, t_list **varlist)
{
	data->i = 0;
	data->j = 0;
	data->pid = fork();
	if (data->pid == -1)
	{
		printf("EXEC_MULTI_CMD: fork 0: fail\n");
		return (1);
	}
	if (data->pid == 0)
	{
		close(data->last_pipe[0]);
		if (data->j < data->n_heredocs && data->i == data->n_cmd[data->j])
			child(data->fds[data->j], data->last_pipe[1], (char **)cmds->content, varlist);
		else
			child(0, data->last_pipe[1], (char **)cmds->content, varlist);
	}
	else if (data->j < data->n_heredocs && data->i == data->n_cmd[data->j])
	{
		if (data->fds[data->j] > 0)
			close(data->fds[data->j]);
		data->j++;
	}
	return (0);
}

static void	manage_child(t_multicmd *data, t_list *cmds, t_list **varlist)
{
	if (data->pid == 0)
	{
		close(data->new_pipe[0]);
		if (data->j < data->n_heredocs && data->i == data->n_cmd[data->j])
			child(data->fds[data->j], data->new_pipe[1], (char **)cmds->content, varlist);
		else
			child(data->last_pipe[0], data->new_pipe[1], (char **)cmds->content, varlist);
	}
	else
	{
		close(data->last_pipe[0]);
		data->last_pipe[0] = data->new_pipe[0];
		data->last_pipe[1] = data->new_pipe[1];
		if (data->j < data->n_heredocs && data->i == data->n_cmd[data->j])
		{
			if (data->fds[data->j] > 0)
				close(data->fds[data->j]);
			data->j++;
		}
	}
}

int	middle_cmds(t_multicmd *data, t_list **cmds, t_list **varlist)
{
	data->i = 0;
	while (++data->i < data->cmds_len - 1)
	{
		if (pipe(data->new_pipe) == -1)
		{
			printf("EXEC_MULTI_CMD: pipe %d fail\n", data->i);
			return (1);
		}
		close(data->last_pipe[1]);
		data->pid = fork();
		if (data->pid == -1)
		{
			printf("EXEC_MULTI_CMD: fork %d: fail\n", data->i);
			return (1);
		}
		/*else if (data->pid == 0)
		{
			close(data->new_pipe[0]);
			if (data->j < data->n_heredocs && data->i == data->n_cmd[data->j])
				child(data->fds[data->j], data->new_pipe[1], (char **)cmds->content, varlist);
			else
				child(data->last_pipe[0], data->new_pipe[1], (char **)cmds->content, varlist);
		}
		else
		{
			close(data->last_pipe[0]);
			data->last_pipe[0] = data->new_pipe[0];
			data->last_pipe[1] = data->new_pipe[1];
			if (data->j < data->n_heredocs && data->i == data->n_cmd[data->j])
			{
				if (data->fds[data->j] > 0)
					close(data->fds[data->j]);
				data->j++;
			}
		}*/
		manage_child(data, *cmds, varlist);
		*cmds = (*cmds)->next;
	}
	return (0);
}

void	last_cmd(t_multicmd *data, t_list *cmds, t_list **varlist)
{
	data->pid = fork();
	if (data->pid == 0)
	{
		if (data->j < data->n_heredocs && data->i == data->n_cmd[data->j])
		{
			child(data->fds[data->j], 1, (char **)cmds->content, varlist);
		}
		else
		{
			child(data->last_pipe[0], 1, (char **)cmds->content, varlist);
		}
	}
	else if (data->j < data->n_heredocs && data->i == data->n_cmd[data->j])
	{
		if (data->fds[data->j] > 0)
			close(data->fds[data->j]);
		data->j++;
	}
	close (data->last_pipe[0]);
}

int	exec_multi_cmd(t_list *cmds, t_list **varlist)
{
	/*int	i;
	int	last_pipe[2];
	int	new_pipe[2];
	int	pid;
	int	cmds_len;
	int	status;
	int	*n_cmd;
	int	*fds;
	int	n_heredocs;
	int	j;*/
	int status;
	t_multicmd data;

	//cmds_len = ft_lstsize(cmds);
	data.cmds_len = ft_lstsize(cmds);
	/*n_heredocs = count_heredoc(cmds);
	if (n_heredocs > 0)
	{
		n_cmd = malloc(sizeof(int) * n_heredocs);
		fds = malloc(sizeof(int) * n_heredocs);
		if (open_heredocs(cmds, &n_cmd, &fds))
		{
			free(n_cmd);
			free(fds);
			return (1);
		}
	}*/
	if (manage_heredocs(&data, cmds))
		return (1);
	if (pipe(data.last_pipe) == -1)
	{
		printf("EXEC_MULTI_CMD: pipe 0: fail\n");
		return (1);
	}
	/*data.i = 0;
	data.j = 0;
	data.pid = fork();
	if (data.pid == -1)
	{
		printf("EXEC_MULTI_CMD: fork 0: fail\n");
		return (1);
	}
	if (data.pid == 0)
	{
		close(data.last_pipe[0]);
		if (data.j < data.n_heredocs && data.i == data.n_cmd[data.j])
			child(data.fds[data.j], data.last_pipe[1], (char **)cmds->content, varlist);
		else
			child(0, data.last_pipe[1], (char **)cmds->content, varlist);
	}
	else if (data.j < data.n_heredocs && data.i == data.n_cmd[data.j])
	{
		if (data.fds[data.j] > 0)
			close(data.fds[data.j]);
		data.j++;
	}*/
	if (first_cmd(&data, cmds, varlist))
		return (1);
	cmds = cmds->next;
	/*data.i = 0;
	while (++data.i < data.cmds_len - 1)
	{
		if (pipe(data.new_pipe) == -1)
		{
			printf("EXEC_MULTI_CMD: pipe %d fail\n", data.i);
			return (1);
		}
		close(data.last_pipe[1]);
		data.pid = fork();
		if (data.pid == -1)
		{
			printf("EXEC_MULTI_CMD: fork %d: fail\n", data.i);
			return (1);
		}
		else if (data.pid == 0)
		{
			close(data.new_pipe[0]);
			if (data.j < data.n_heredocs && data.i == data.n_cmd[data.j])
				child(data.fds[data.j], data.new_pipe[1], (char **)cmds->content, varlist);
			else
				child(data.last_pipe[0], data.new_pipe[1], (char **)cmds->content, varlist);
		}
		else
		{
			close(data.last_pipe[0]);
			data.last_pipe[0] = data.new_pipe[0];
			data.last_pipe[1] = data.new_pipe[1];
			if (data.j < data.n_heredocs && data.i == data.n_cmd[data.j])
			{
				if (data.fds[data.j] > 0)
					close(data.fds[data.j]);
				data.j++;
			}
		}
		cmds = cmds->next;
	}*/
	if (middle_cmds(&data, &cmds, varlist))
		return (1);
	close(data.last_pipe[1]);
	/*data.pid = fork();
	if (data.pid == 0)
	{
		if (data.j < data.n_heredocs && data.i == data.n_cmd[data.j])
		{
			child(data.fds[data.j], 1, (char **)cmds->content, varlist);
		}
		else
		{
			child(data.last_pipe[0], 1, (char **)cmds->content, varlist);
		}
	}
	else if (data.j < data.n_heredocs && data.i == data.n_cmd[data.j])
	{
		if (data.fds[data.j] > 0)
			close(data.fds[data.j]);
		data.j++;
	}
	close (data.last_pipe[0]);*/
	last_cmd(&data, cmds, varlist);
	while (wait(&status) != -1)
		;
	if (data.n_heredocs > 0)
	{
		free (data.n_cmd);
		free(data.fds);
	}
	return (WEXITSTATUS(status));
}

/*  exec_cmd:
		Executes each command from the "cmds" list.
		If there is only one command, it will make a fork to execute it unless
		the command is a builtin.
		Otherwise it will make a fork for each command no matter if it's a
		builtin or not.
	parameters:
		cmds: linked list containing arrays of strings. Every element of the
			list represents a command, and every string inside the cmd is an
			argument.
		env: linked list where each element is a struct containing the name and
			the content of each variable in the environment.
	Return:
		Exit status of the last function executed
	Todo:
		[ ] Create a tester
		[ ] OK function names
		[x] Convert env to an array (method from t_var)
		[x] If 1 cmd and is builtin, don't fork it
		[x] If 1 cmd and not builtin, fork the command
		[ ] Execute builtins with multiple commands (child)
		[x] return exit status
		[x] Pipes link between forks
		[x] Leaks (no malloc in this func)
		[x] close pipes
		[ ] close files (child)
		[ ] norminette
*/
int	exec_cmd(t_list *cmds, t_list **varlist)
{
	int		status;

	if (!cmds)
	{
		printf("EXEC_CMD: no cmds\n");
		return (1);
	}
	if (ft_lstsize(cmds) == 1)
		status = exec_one_cmd((char **)cmds->content, varlist);
	else
		status = exec_multi_cmd(cmds, varlist);
	return (status);
}

/*int main(int c, char **v, char **e)
{
	if (c == 1)
		return (printf("1 arg\n"), 1);
	t_list *env = array_to_varlist(e);
	t_list *cmd = parse(v[1], NULL, env);
	return (exec_cmd(cmd, env));
}*/
