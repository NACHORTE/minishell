/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscar <oscar@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:56:41 by oscar             #+#    #+#             */
/*   Updated: 2023/09/20 22:33:15 by oscar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"




/*  If the command given is a builtin, then no fork must be made and executes
    the function for builtins. If it is a normal command, creates a fork and
    calls the child process setting infile=stdin, outfile=stdout
    If the command contains redirections, they will be handled inside the child
*/
void exec_one_cmd(char **cmd, char **env)
{
    if (check_builtin)
        return;
    int pid = fork();
    if (pid == -1)
        algo de error;
    if (!pid)
        child(0, 1, cmd ,env);
}

void exec_multi_cmd(t_list *cmds, char **env)
{
	int i = 0;
	int last_pipe[2];
	int new_pipe[2];
	int pid;
    int cmds_len;

    cmds_len = ft_lstsize(cmds);
	pipe(last_pipe);
	pid = fork();
	if (pid == 0)
	{
		close(last_pipe[0]);
		child(0, last_pipe[1], (char **)cmds->content, path);
	}
    cmds = cmds->next;
	while (++i < cmds_len - 1)
	{
		pipe(new_pipe);
		pid = fork();
		if (pid == 0)
		{
			close(last_pipe[1]);
			close(new_pipe[0]);
			child(last_pipe[0], new_pipe[1], (char **)cmds->content, path);
		}
		else
		{
			close(last_pipe[0]);
			close(last_pipe[1]);
			last_pipe[0] = new_pipe[0];
			last_pipe[1] = new_pipe[1];
		}
        cmds = cmds->next;
	}
	pid = fork();
	close(last_pipe[1]);
	if (pid == 0)
	{
		child(last_pipe[0], 1, (char **)cmds->content, path);
	}
	close (last_pipe[0]);
	while (wait(NULL) != -1)
	    ;
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
		Nothing :)
	Todo:
		[ ] Create a tester
		[ ] OK function names
		[ ] Convert env to an array (method from t_var)
		[ ] If 1 cmd and is builtin, don't fork it
		[ ] If 1 cmd and not builtin, fork the command
		[ ] Execute builtins with multiple commands
		[ ] Pipes link between forks
		[ ] Leaks
		[ ] close pipes, files
		[ ] norminette
*/
void    exec_cmd(t_list *cmds, t_list *env)
{
    char    **env_array;
    if (!cmds)
        return ;
    env_array = env_to_array(env);
    if (ft_lstsize(cmds) == 1)
        exec_one_cmd((char **)cmds->content, env_array);
    else
        exec_multi_cmd(cmds, env_array);
}
