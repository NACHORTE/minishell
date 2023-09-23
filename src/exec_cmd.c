/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 13:56:41 by oscar             #+#    #+#             */
/*   Updated: 2023/09/23 20:55:13 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <sys/types.h>	//XXX
#include <sys/wait.h>	//XXX
#include <semaphore.h>
#include <fcntl.h>

/*void	child(int infile, int outfile, char **cmd, char **env, sem_t *sem)
{
	sem_wait(sem);
	printf("CHILD:\n\tinfile=%d\n\toutfile=%d\n", infile, outfile);
	sem_post(sem);
	sem_close(sem);
	exit (0);
}*/

/*  If the command given is a builtin, then no fork must be made and executes
    the function for builtins. If it is a normal command, creates a fork and
    calls the child process setting infile=stdin, outfile=stdout
    If the command contains redirections, they will be handled inside the child
*/
int	exec_one_cmd(char **cmd, char **env, t_command *global)
{
	int	status;

    //if (check_builtin(cmd, env))
    //    return;
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
	printf("EXEC_MULTI_CMD: pipe 0: read %d, write %d\n",last_pipe[0], last_pipe[1]);
	pid = fork();
	if (pid == -1)
	{
		printf("EXEC_MULTI_CMD: fork 0: fail\n");
		return 1;
	}
	if (pid == 0)
	{
		close(last_pipe[0]);
		child(last_pipe[0], new_pipe[1], (char **)cmds->content, global);
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
		printf("EXEC_MULTI_CMD: pipe %d: read %d, write %d\n", i, new_pipe[0], new_pipe[1]);
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
		child(last_pipe[0], new_pipe[1], (char **)cmds->content, global);
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
		Nothing :)
	Todo:
		[ ] Create a tester
		[ ] OK function names
		[ ] Convert env to an array (method from t_var)
		[ ] If 1 cmd and is builtin, don't fork it
		[ ] If 1 cmd and not builtin, fork the command
		[ ] Execute builtins with multiple commands
		[ ] return exit status on some variable
		[ ] Pipes link between forks
		[ ] Leaks
		[ ] close pipes, files
		[ ] norminette
*/
int    exec_cmd(t_list *cmds, t_list *env, t_command *global)
{
    char    **env_array;

	if (!cmds)
	{
		printf("EXEC_CMD: no cmds\n");
		return 1;
	}
    env_array = varlist_to_array(env);
    if (ft_lstsize(cmds) == 1)
        return (exec_one_cmd((char **)cmds->content, env_array, global));
    else
        return (exec_multi_cmd(cmds, env_array, global));
}

/*int main(int c, char **v, char **e)
{
	if (c == 1)
		return (printf("1 arg\n"), 1);
	sem_t *sem = sem_open("my_semaphore", O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }
	(void)c;
	t_list *env = array_to_varlist(e);
	t_list *cmd = parse(v[1], NULL, env);
	return (exec_cmd(cmd, env,sem));
}*/