/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:56:41 by oscar             #+#    #+#             */
/*   Updated: 2023/10/06 12:24:59 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_cmd.h"
#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

static void	check_doublestdin_here(char *character, int *fd)
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

int	check_restdin_here(char **input)
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
			check_doublestdin_here(&input[i][j + 1], &fd);
		}
		i++;
	}
	return (fd);
}

//[ ] check open files and leaks
int	exec_one_builtin(t_cmd *cmd, t_list **varlist, int *status)
{
	int		std_in;
	int		std_out;
	char	**cmd_parsed;

	cmd_parsed = (char **)cmd->args;
	if (!is_builtin(cmd_parsed[0]))
	{
		return (0);
	}
	std_in = dup(0);
	std_out = dup(1);
	if (cmd->infile > 1)
		dup2(cmd->infile, 0);
	if (cmd->outfile > 1)
		dup2(cmd->outfile, 1);
	*status = exec_builtin(cmd_parsed, varlist);
	if (cmd->infile > 1)
		close(cmd->infile);
	if (cmd->outfile > 1)
		close(cmd->outfile);
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
int	exec_one_cmd(t_cmd *cmd, t_list **varlist)
{
	int	status;
	int	pid;

	if (cmd->infile < 0 || cmd->outfile < 0)
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
		child(cmd->infile, cmd->outfile, cmd->args, varlist);
	if (cmd->infile > 0)
		close(cmd->infile);
	if (cmd->outfile > 1)
		close(cmd->outfile);
	wait(&status);
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

	signal(SIGINT, &sig_child);
	if (!cmds)
	{
		printf("EXEC_CMD: no cmds\n");
		return (1);
	}
	if (ft_lstsize(cmds) == 1)
		status = exec_one_cmd((t_cmd *)cmds->content, varlist);
	else
		status = exec_multi_cmd(cmds, varlist);
	signal(SIGINT, &new_line);
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
