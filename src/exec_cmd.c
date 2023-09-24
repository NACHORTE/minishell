/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:56:41 by oscar             #+#    #+#             */
/*   Updated: 2023/09/24 16:27:42 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	exec_one_builtin(char **cmd, t_list **local, t_list **env, int *status)
{
	int	std_in;
	int	std_out;
	int	out;
	char **cmd_parsed;

	std_in = dup(0);
	std_out = dup(1);
    redirect_streams(0, 1, cmd);
    cmd_parsed = parse_cmd(cmd); //XXX cambiale el nombre machito
	out = exec_builtin(cmd_parsed, local, env, status);
	ft_array_free(cmd_parsed);
	dup2(std_in, 0);
	dup2(std_out, 1);
	close(std_in);
	close(std_out);
	return (out);
}

/*  If the command given is a builtin, then no fork must be made and executes
    the function for builtins. If it is a normal command, creates a fork and
    calls the child process setting infile=stdin, outfile=stdout
    If the command contains redirections, they will be handled inside the child
*/
int	exec_one_cmd(char **cmd, char **env, t_command *global)
{
	int	status;

    if (exec_one_builtin(cmd, &global->local, &global->env, &status))
        return (status);
    int pid = fork();
    if (pid == -1)
	{
        printf("EXEC_ONE_CMD: fork fail\n");
		return 1;
	}
	if (pid == 0)
        child(0, 1, cmd, global);
	wait(&status);
	return (status);
}

int exec_multi_cmd(t_list *cmds, char **env, t_command *global)
{
	int i;
	int last_pipe[2];
	int new_pipe[2];
	int pid;
    int cmds_len;
	int	status;

    cmds_len = ft_lstsize(cmds);
	if (pipe(last_pipe) == -1)
	{
		printf("EXEC_MULTI_CMD: pipe 0: fail\n");
		return  1;
	}
	//printf("EXEC_MULTI_CMD: pipe 0: read %d, write %d\n",last_pipe[0], last_pipe[1]);
	pid = fork();
	if (pid == -1)
	{
		printf("EXEC_MULTI_CMD: fork 0: fail\n");
		return 1;
	}
	if (pid == 0)
	{
		close(last_pipe[0]);
		child(0, last_pipe[1], (char **)cmds->content, global);
	}
    cmds = cmds->next;
	i = 0;
	while (++i < cmds_len - 1)
	{
		if (pipe(new_pipe) == -1)
		{
			printf("EXEC_MULTI_CMD: pipe %d fail\n",i);
			return 1;
		}
		close(last_pipe[1]);
		//printf("EXEC_MULTI_CMD: pipe %d: read %d, write %d\n", i, new_pipe[0], new_pipe[1]);
		pid = fork();
		if (pid == -1)
		{
			printf("EXEC_MULTI_CMD: fork %d: fail\n", i);
			return 1;
		}
		else if (pid == 0)
		{
			close(new_pipe[0]);
			child(last_pipe[0], new_pipe[1], (char **)cmds->content, global);
		}
		else
		{
			close(last_pipe[0]);
			last_pipe[0] = new_pipe[0];
			last_pipe[1] = new_pipe[1];
		}
        cmds = cmds->next;
	}
	close(last_pipe[1]);
	pid = fork();
	if (pid == 0)
	{
		child(last_pipe[0], 1, (char **)cmds->content, global);
	}
	close (last_pipe[0]);
	while (wait(&status) != -1)
	    ;
	return (status);
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
int    exec_cmd(t_list *cmds, t_list *env, t_command *global)
{
    char    **env_array;
	int		status;

	if (!cmds)
	{
		printf("EXEC_CMD: no cmds\n");
		return (1);
	}
    env_array = varlist_to_array(env, 1);
    if (ft_lstsize(cmds) == 1)
        status = exec_one_cmd((char **)cmds->content, env_array, global);
    else
        status = exec_multi_cmd(cmds, env_array, global);
	ft_array_free(env_array);
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
