/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rm_assignation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 14:31:36 by iortega-          #+#    #+#             */
/*   Updated: 2023/10/06 14:51:25 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_assignation(char *cmd)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (cmd[i])
	{
		if (i != 0)
		{
			if (cmd[i] == '=')
			{
				if (cmd[i + 1])
				{
					flag = 1;
					break ;
				}
			}
		}
		i++;
	}
	return (flag);
}

static char	**size_cmds(char **cmds)
{
	int		i;
	char	**out;

	i = 0;
	while (cmds[i])
		i++;
	out = malloc(sizeof(char *) * (i + 1));
	if (!out)
		return (NULL);
	return (out);
}

static void	copy_strings(char **in, char **out)
{
	while (*in)
	{
		*out = *in;
		out++;
		in++;
	}
}

void	redirect(int infile, int outfile)
{
	if (infile > 1)
	{
		dup2(infile, 0);
		close(infile);
	}
	if (outfile > 1)
	{
		dup2(outfile, 1);
		close(outfile);
	}
}

char	**rm_assignation(char **cmd)
{
	int		i;
	char	**out;

	i = 0;
	while (cmd[i] && is_assignation(cmd[i]))
	{
		free(cmd[i]);
		i++;
	}
	if (i == 0)
		return (cmd);
	out = size_cmds(&cmd[i]);
	if (!out)
		exit(return_msg("Could not allocate memory", 2, 23));
	copy_strings(&cmd[i], out);
	free(cmd);
	return (out);
}
