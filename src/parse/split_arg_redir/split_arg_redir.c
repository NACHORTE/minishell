/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_arg_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 20:56:54 by orudek            #+#    #+#             */
/*   Updated: 2023/10/06 15:31:32 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

static int	arg_len(char *s)
{
	int	i;
	int	state;

	i = -1;
	state = 0;
	while (s[++i]
		&& (state || (s[i] != ' ' && s[i] != '<' && s[i] != '>')))
	{
		if (state == 0 && (s[i] == '\'' || s[i] == '"'))
			state = (s[i] == '\'') * 2 + (s[i] == '"');
		else if ((state == 1 && s[i] == '"') || (state == 2 && s[i] == '\''))
			state = 0;
	}
	return (i);
}

static int	redir_len(char *cmd)
{
	int	i;

	i = 1;
	if (cmd[i] == '<' || cmd[i] == '>')
		i++;
	while (cmd[i] == ' ')
		i++;
	i += arg_len(&cmd[i]);
	return (i);
}

static char	*get_arg(char **cmd)
{
	int		len;
	char	*out;

	if (**cmd == '>' || **cmd == '<')
		len = redir_len(*cmd);
	else
		len = arg_len(*cmd);
	out = malloc(sizeof(char) * (len + 1));
	if (!out)
		return ((void *)return_msg("Couldn't allocate memory", 2, 0));
	ft_strlcpy(out, *cmd, len + 1);
	(*cmd) += len;
	return (out);
}

static t_arg_redir	*get_cmd(char *cmd)
{
	t_arg_redir	*out;
	char		*arg;

	out = new_arg_redir();
	if (!out)
		return ((void *)return_msg("Couldn't allocate memory", 2, 0));
	while (*cmd)
	{
		while (*cmd == ' ')
			cmd++;
		if (!*cmd)
			break ;
		arg = get_arg(&cmd);
		if (!arg || ((arg[0] == '<' || arg[0] == '>')
				&& !ft_lstadd_back_content(&out->redir, arg))
			|| (arg[0] != '<' && arg[0] != '>'
				&& !ft_lstadd_back_content(&out->args, arg)))
		{
			free_arg_redir(out);
			return ((void *)return_msg("Couldn't allocate memory", 2, 0));
		}
	}
	return (out);
}

t_list	*split_arg_redir(char **cmd)
{
	t_list		*out;
	t_arg_redir	*new_cmd;

	out = NULL;
	while (*cmd)
	{
		new_cmd = get_cmd(*cmd);
		if (!new_cmd || !ft_lstadd_back_content(&out, new_cmd))
		{
			ft_lstfree(out, free_arg_redir);
			if (new_cmd)
				free_arg_redir(new_cmd);
			return ((void *)return_msg("Couldn't allocate memory", 2, 0));
		}
		cmd++;
	}
	return (out);
}
