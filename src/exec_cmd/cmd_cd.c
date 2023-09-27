/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 21:26:02 by orudek            #+#    #+#             */
/*   Updated: 2023/09/27 22:47:36 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_cmd.h"
/*
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

//[ ] cd busca tambien en en*/
int	cmd_cd(char **cmd, t_list **env)   //NOTE Hacer un tester para esto
{
	(void)cmd;
	(void)env;
	return (0);
/*	char	*old_pwd;
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
	return (0);*/
}
