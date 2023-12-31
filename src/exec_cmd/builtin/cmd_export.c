/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 21:04:33 by orudek            #+#    #+#             */
/*   Updated: 2023/10/02 14:49:00 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_cmd.h"

static int	get_name(char *str, char **name)
{
	int		len;
	char	*new_str;

	len = 0;
	while (str[len] != '=' && str[len] != 0)
		len++;
	new_str = malloc(len + 1);
	if (!new_str)
		return (0);
	*name = new_str;
	ft_strlcpy(*name, str, len + 1);
	if (!is_varname_ok(*name))
	{
		write(2, "export: `", 9);
		write(2, *name, ft_strlen(*name));
		write(2, "': not a valid identifier\n", 26);
		free(*name);
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
	int		len;
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

static char	str_to_var(char *cmd, char **name, char **content)
{
	if (!get_name(cmd, name))
		return (0);
	if (!get_content(cmd, content))
	{
		free(*name);
		return (0);
	}
	return (1);
}

int	cmd_export(char **cmd, t_list **varlist)
{
	int		i;
	int		exit_status;
	t_var	var;
	int		index;

	i = 0;
	exit_status = 0;
	while (cmd[++i])
	{
		if (!str_to_var(cmd[i], &var.name, &var.content))
		{
			exit_status = 1;
			continue ;
		}
		index = is_in_varlist(*varlist, var.name);
		if (index != -1 && var.content == NULL)
			((t_var *)ft_lstget_val(*varlist, index))->type = ENV_VAR;
		else if (!set_variable(varlist, var.name, var.content, ENV_VAR))
			exit_status = 1;
		free(var.name);
		if (var.content)
			free(var.content);
	}
	return (exit_status);
}
