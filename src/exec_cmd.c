/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscar <oscar@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:56:41 by oscar             #+#    #+#             */
/*   Updated: 2023/09/20 21:53:35 by oscar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//[ ] Change function names
//[ ] Remove creating files functionality from parse_cmd
//[ ] Create a tester for exec_cmd

void child(int infile, int outfile, char **cmd, char **env)
{
    //makes the needed dup2, creates the files if there are multiple output redirections
	// and opens the correct file.
    redirect_streams(infile, outfile, cmd); 
    //removes the redirections from the command returning a new char **array
    cmd_parsed = parse_cmd(); 
    //gets the full path of the command
    cmd_path = get_cmd_path();
    excve(cmd_path, cmd_parsed, env);
	perror(cmd_parsed[0]);
	exit(errno); //NOTE maybe just exit 1 is OK
}
/*  If the command given is a builtin, then no fork must be made and executes
    the function for builtins. If it is a normal command, creates a fork and
    calls the child process setting infile=stdin, outfile=stdout
    If the command contains redirections, they will be handled inside the child
*/
void exec_one_cmd(char **cmd, char **path)
{
    if (check_builtin)
        return;
    int pid = fork();
    if (pid == -1)
        algo de error;
    if (!pid)
        child(0, 1, cmd ,path);
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
/*  This function only executes if the command is not an assignation
    first it removes all the assignations before the command
    If the size of the command list is 1, then execute the special function for
    the case, and if it is greater than one, execute the general function for
    executing commands
*/
void    exec_cmd(t_list *cmds, t_list *env, t_list *local)
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
