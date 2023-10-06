/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:51:13 by iortega-          #+#    #+#             */
/*   Updated: 2023/10/06 15:41:04 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include <fcntl.h>
#include "minishell.h"

static char	**save_args(t_list *args)
{
	char	**out;
	int		i;

	i = ft_lstsize(args);
	out = malloc(sizeof(char *) * (i + 1));
	if (!out)
		return (NULL);
	out[i] = 0;
	i = 0;
	while (args)
	{
		out[i] = ft_strdup((char *)args->content);
		if (!out[i])
		{
			ft_array_free(out);
			return (NULL);
		}
		i++;
		args = args->next;
	}
	return (out);
}

static t_cmd	*get_cmd(t_arg_redir *cmd)
{
	t_cmd	*out;

	out = malloc(sizeof(t_cmd));
	if (!out)
		return (NULL);
	out->infile = check_restdin_here((t_list *)cmd->redir);
	if (out->infile < 0)
	{
		free(out);
		return (NULL);
	}
	redirect_streams(out, (t_list *)cmd->redir);
	out->args = save_args((t_list *)cmd->args);
	if (!out->args)
		return (NULL);
	return (out);
}

t_list	*cmd_redir(t_list *pipes)
{
	t_cmd	*cmd;
	t_list	*out;

	out = NULL;
	while (pipes)
	{
		cmd = get_cmd((t_arg_redir *)pipes->content);
		if (!cmd || !ft_lstadd_back_content(&out, cmd))
		{
			ft_lstfree(out, free_cmd);
			return ((void *)return_msg("Could not allocate memory", 2, 0));
		}
		pipes = pipes->next;
	}
	return (out);
}
