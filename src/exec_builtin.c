/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 12:22:38 by orudek            #+#    #+#             */
/*   Updated: 2023/09/25 22:46:27 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

int	cmd_pwd()
{
	char	*dir;

	//NOTE: FIRST try saving in buffer with #include <limits.h> and buffer[PATH_MAX];
	// if i doesn't work do the malloc;
	dir = getcwd(NULL, 0);
	if (dir == NULL)
	{
		printf("ERROR: PWD\n"); //[ ]: define.h
		return (1);
	}
	printf("%s\n", dir);
	free (dir);
	return (0);
}

int	cmd_cd(char **cmd, t_list **env)   //NOTE Hacer un tester para esto
{
	char	*old_pwd;
	char	*curr_pwd;
	char	*home;
	char	*dst;
	
	if (cmd[1] != 0 && cmd[2] != 0)
	{
		printf("cd: too many arguments\n");
		return (1);
	}
	if (!get_variable(*env, "PWD", &old_pwd))
		old_pwd = NULL;
	if (!cmd[1] || !ft_strcmp(cmd[1], "~"))
	{
		if (get_variable(*env, "HOME", home))
			chdir(home);
		else
		{
			printf("cd: HOME not set\n");
			return (1);
		}
	}
	else if (!ft_strcmp(cmd[1], "-"))    //COMPROBAR QUE HAY OLDPWD
	{
		if (!get_variable(*env, "OLDPWD", &dst))
		{
			printf("cd: OLDPWD not set\n");
			return (1);
		}
		chdir(dst);
	}
	else
	{
		if (chdir(cmd[1]) != 0)
		{
			perror(cmd[1]);
			return (1);
		}
	}
	curr_pwd = getcwd(NULL, 0);
	if (!curr_pwd)
	{
		printf("cd: Cannot allocate memory\n");
		return (1);
	}
	if (!set_variable(env, "OLDPWD", old_pwd))
	{
		free(old);
		free(old_pwd);
		return (1);
	}
	if (!set_variable(env, "PWD", curr_pwd))
	{
		free(curr);
		free(curr_pwd);
		return (1);
	}
	return (0);
}

int	cmd_env(t_list *env)
{
	//NOTE leaks after using env
	//[x] Don't print variables with content=NULL
	while (env)
	{
		if (((t_var *)env->content)->content);
			printf("%s=%s\n", ((t_var *)env->content)->name,
				((t_var *)env->content)->content);
		env = env->next;
	}
	return (0);
}

int	cmd_echo(char **cmd)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	if (!ft_strcmp(cmd[1], "-n"))
	{
		flag = 1;
		i++;
	}
	while(cmd[++i])
	{
		printf("%s", cmd[i]);
		if (cmd[i + 1])
			printf(" ");
	}
	if (flag == 0)
		printf("\n");
	return (0);
}

int	cmd_unset(char **cmd, t_list **local, t_list **env)
{
	while (*cmd)
	{
		if (get_variable(*local, cmd[0], NULL))
			unset_variable(local, cmd[0]);
		if (get_variable(*env, cmd[0], NULL))
			unset_variable(env, cmd[0]);
		cmd++;
	}
	return (0);
}

void	cmd_exit()
{
	printf("exit\n");
	exit(0);
}
///////////////
/*	get_name:
		Receives a str with the format name=content or name and duplicates
		the part of the string of the name into the name variable.
		Wrong names are not checked.
		If something goes wrong, return 0. On exit, returns 1.
*/
static int	get_name(char *str, char **name)
{
	int	len;
	char *new_str;
	
	len = 0;
	while (str[len] != '=' && str[len] != 0)
		len++;
	new_str= malloc(len + 1);
	if (!new_str)
		return (0);
	*name = new_str;
	ft_strlcpy(*name, str, len + 1);
	return (1);
}
/*	get_content:
		Receives a str with the format name=content or name and duplicates
		the part of the string of the content into the content variable.
		If str has no '=' in it, saves NULL into the content.
		If something goes wrong, return 0. On exit, returns 1.
*/
static int	get_content(char *str, char **content)
{
	int	len;
	char	*new_str;

	while (*str != '=' && *str != 0)
		str++;
	if (*str != '=')
	{
		*content = NULL;
		return (1);
	}
	len = 0;
	while (str[len] != 0)
		len++;
	new_str = malloc(len + 1);
	if (!new_str)
		return (0);
	*content = new_str;
	ft_strlcpy(*content, str + 1, len + 1);
	return (1);
}

int	cmd_export(char **cmd, t_list **local, t_list **env)
{
	char	*name;
	char	*content;
	int		i;
	int		exit_status;

	i = 0;
	exit_status = 0;
	while (cmd[++i])
	{
		if (!get_name(cmd[i], &name)) //[x] get_name, returns the first half of the assignation
		{
			exit_status = 1;
			continue ;
		}
		if (!is_varname_ok(name))
		{
			free(name);
			exit_status = 1;
			continue ;
		}
		if (get_variable(*local, name, &content))
		{
			if (content != NULL)
			{
				content = ft_strdup(content);
				if (!content)
				{
					exit_status = 1;
					free(name);
					continue ;
				}
			}
			unset_variable(local, name);
			if (!set_variable(env, name, content))
			{
				exit_status = 1;
				free(name);
				if (content)
					free(content);
			}
		}
		else
		{
			if (!get_content(cmd[i], &content))// [x] get_content, returns the second half of the assignation (NULL if there is nothing)
			{
				exit_status = 1;
				free(name);
				continue ;
			}
			if (!set_variable(env, name, content))
			{
				exit_status = 1;
				free(name);
				if (content) //content can be NULL
					free(content);
			}
		}
	}
	return (exit_status);
}

int	is_builtin(char *cmd) //NOTE no se si me gusta está funcion.
{
	if (!cmd || !*cmd)
		return (0);
	if (!ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "export"))
		return (1);
	return (0);
}

int	exec_builtin(char **cmd, t_list **local, t_list **env)
{
	if (!cmd || !*cmd)
		return (1);
	if (!ft_strcmp(cmd[0], "pwd"))
		return (cmd_pwd());
	else if (!ft_strcmp(cmd[0], "cd"))
		return (cmd_cd(cmd, env));
	else if (!ft_strcmp(cmd[0], "env"))
		return (cmd_env(*env));
	else if (!ft_strcmp(cmd[0], "echo"))
		return (cmd_echo(cmd));
	else if (!ft_strcmp(cmd[0], "unset"))
		return (cmd_unset(cmd, local, env));
	else if (!ft_strcmp(cmd[0], "exit"))
		cmd_exit();
	else if (!ft_strcmp(cmd[0], "export"))
		return (cmd_export(cmd, local, env));
	return (1);
}
