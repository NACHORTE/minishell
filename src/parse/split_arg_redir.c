/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_arg_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 20:56:54 by orudek            #+#    #+#             */
/*   Updated: 2023/10/04 16:09:07 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

int	arg_len(char *s)
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

int	redir_len(char *cmd)
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

char	*get_arg(char **cmd)
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

t_arg_redir	*get_cmd(char *cmd)
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
			break;
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
			return ((void *)return_msg("Couldn't allocate memory", 2, 0));
		}
		cmd++;
	}
	return (out);
}

/*
void print_arg_redir(t_list *lst)
{
	for (lst; lst; lst = lst->next)
	{
		printf("List: (%p)\n", lst);
		printf("\targs:\n");
		t_list *args = ((t_arg_redir *)lst->content)->args;
		for (args; args; args = args->next)
			printf("\t\t[%s]\n", (char *)args->content);
		printf("\tredir:\n");
		t_list *redir = ((t_arg_redir *)lst->content)->redir;
		for (redir; redir; redir = redir->next)
			printf("\t\t[%s]\n", (char *)redir->content);
	}
	printf("List: (%p)\n", lst);
}

void print_cmd(t_list *lst)
{
	printf("printeo nacho\n");
	for (lst; lst; lst = lst->next)
	{
		char **args = ((t_cmd *)lst->content)->args;
		for (int i = 0; args[i]; i++)
		{
			printf("%s\n", args[i]);
		}
		printf("infile %d outfile %d\n\n", ((t_cmd *)lst->content)->infile, ((t_cmd *)lst->content)->outfile);
	}
}

int main()
{
	char *cmd[] =
	{
		"hola que tal >>nacho es feo",
		"Como estás hoy <<variable >outfile",
		"cat Makefile",
		"<<último >>comandito",
		NULL
	};
	int i=0;
	for (i;cmd[i];i++)
		printf("[%s]\n",cmd[i]);
	printf("[%s]\n",cmd[i]);
	t_list *result = split_arg_redir(cmd);
	print_arg_redir(result);
	t_list *nacho = cmd_redir(result);
	print_cmd(nacho);
	ft_lstfree(result, free_arg_redir);
	ft_lstfree(nacho, free_cmd);
	exit (0);
}
*/