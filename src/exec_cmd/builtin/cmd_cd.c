/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 21:26:02 by orudek            #+#    #+#             */
/*   Updated: 2023/10/02 14:04:02 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_cmd.h"

int	cd(char *str, t_list **varlist)
{
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return (1);
	if (chdir(str) == -1)
	{
		perror(str);
		free(old_pwd);
		return (1);
	}
	if (!set_variable(varlist, "OLDPWD", old_pwd, DEFAULT_VAR))
		return (1);
	free(old_pwd);
	return (0);
}

static int	cd_home(t_list **varlist)
{
	t_var	*var;

	if (is_in_varlist(*varlist, "HOME") != -1)
	{
		if (get_variable(*varlist, "HOME", &var) == -1)
			return (1);
		if (cd(var->content, varlist))
		{
			free_var(var);
			return (1);
		}
		free_var(var);
		return (0);
	}
	printf("cd: HOME not set\n");
	return (1);
}

static int	cd_back(t_list **varlist)
{
	t_var	*var;

	if (is_in_varlist(*varlist, "OLDPWD") != -1)
	{
		if (get_variable(*varlist, "OLDPWD", &var) == -1)
			return (1);
		if (cd(var->content, varlist))
		{
			free_var(var);
			return (1);
		}
		free_var(var);
		return (0);
	}
	printf("cd: OLDPWD not set\n");
	return (1);
}

//[ ] COMENTAR
//[ ] Hacer un tester para esto
int	cmd_cd(char **cmd, t_list **varlist)
{
	if (cmd[1] != 0 && cmd[2] != 0)
	{
		write(2, "cd: too many arguments\n", 23);
		return (1);
	}
	if (!cmd[1] || !ft_strcmp(cmd[1], "~"))
		return (cd_home(varlist));
	else if (cmd[1] && !ft_strcmp(cmd[1], "-"))
		return (cd_back(varlist));
	else
		return (cd(cmd[1], varlist));
}
