/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 12:22:38 by orudek            #+#    #+#             */
/*   Updated: 2023/09/27 20:35:21 by orudek           ###   ########.fr       */
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
/////////////////////////////////


static	char	update_pwd(char *old_pwd, t_lsit **env)
{
	
}

static char	cd_home(char **cmd, t_list **env)
{
	int		exit_status;
	char	*home;

	if (!cmd[1] || !ft_strcmp(cmd[1], "~"))
	{
		home = NULL;
		exit_status = get_variable(*env, "HOME", &home);
		if (exit_status == 1 && home)
		{
			if (chdir(home) == -1)
			{
				perror(home);
				exit_status = -1;
			}
			free(home);
			return (exit_status);
		}		
		if (exit_status == 0 || !home) 
			printf("cd: HOME not set\n");
		return (-1);
	}
	return (0);
}

static char	cd_back(char **cmd, t_list **env)
{
	int		exit_status;
	char	*old_pwd;

	if (!cmd[1] || !ft_strcmp(cmd[1], "~"))
	{
		old_pwd = NULL;
		exit_status = get_variable(*env, "OLDPWD", &old_pwd);
		if (exit_status == 1 && old_pwd)
		{
			if (chdir(old_pwd) == -1)
			{
				perror(old_pwd);
				exit_status = -1;
			}
			printf("%s\n", old_pwd);
			free(old_pwd);
			return (exit_status);
		}		
		if (exit_status == 0 || !old_pwd) 
			printf("cd: OLDPWD not set\n");
		return (-1);
	}
	return (0);
}

static char	cd_dir(char **cmd, t_list **env)
{
	if (chdir(cmd[1]) == -1)
	{
		perror(cmd[1]);
		return (-1);
	}
	return (1);
}

//[ ] cd busca tambien en en
int	cmd_cd(char **cmd, t_list **env)   //NOTE Hacer un tester para esto
{
	char	*old_pwd;
	char	*curr_pwd;
	char	*dst;
	
	if (cmd[1] != 0 && cmd[2] != 0)
	{
		printf("cd: too many arguments\n");
		return (1);
	}
	if (cd_home(cmd, env) != 0)
	else if (cd_back(cmd, env) != 0)
	else if (cd_dir(cmd, env) != 1)
		return (1);
	if (!curr_pwd)
	{
		printf("cd: Cannot allocate memory\n");
		return (1);
	}
	if (!set_variable(env, "OLDPWD", old_pwd))
	{
		free(old_pwd);
		return (1);
	}
	if (!set_variable(env, "PWD", curr_pwd))
	{
		free(curr_pwd);
		return (1);
	}
	return (0);
}
////////////////////////////////////////
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
		if (get_variable(*local, cmd[0], NULL) == 1)
			unset_variable(local, cmd[0]);
		if (get_variable(*env, cmd[0], NULL) == 1)
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
	if (!is_varname_ok(*name))
	{
		free(name);
		return (0);
	}
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
	str++;
	while (str[len] != 0)
		len++;
	new_str = malloc(len + 1);
	if (!new_str)
		return (0);
	*content = new_str;
	ft_strlcpy(*content, str, len + 1);
	return (1);
}

char	str_to_var(char *cmd, char **name, char **content)
{
	if (!get_name(str, name))
		return (0);
	if (!get_content(str, content))
	{
		free(*name);
		return (0);
	}
	return (1);
}

int update_from_local(t_list **local, t_var *var)
{
	char	*last_content;
	char	get_var_status;

	get_var_status = get_variable(*local, var->name, &last_content);
	if (get_var_status == -1)
		return (-1);
	if (get_var_status == 1)
	{
		unset_variable(local, var->name);
		if (!var->content)
			var->content = last_content;
		else
			free(last_content);
	}
	return (get_var_status);
}

int	cmd_export(char **cmd, t_list **local, t_list **env)
{
	int		i;
	int		exit_status;
	t_var	var;
	char	*last_content;

	i = 0;
	exit_status = 0;
	while (cmd[++i])
	{
		if (str_to_var(cmd[i], &var.name, &var.content))
		{
			exit_status = 1;
			continue ;
		}
		if (update_from_local(local, &var) == -1)
			|| !set_variable(env, var.name, var.content))
			exit_status = 1;
		free(var.name);
		if (var.content)
			free(var.content);
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
