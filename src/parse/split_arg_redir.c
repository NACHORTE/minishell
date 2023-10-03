/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_arg_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 20:56:54 by orudek            #+#    #+#             */
/*   Updated: 2023/10/03 23:23:14 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

int	arg_len(char *s)
{
	int i;
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
	int i;

	i = 1;	//If this func is called the first char must be < or >
	if (cmd[i] == '<' || cmd[i] == '>') //If second char is < or > add 1
		i++;
	while (cmd[i] == ' ') //count all the spaces
		i++;
	i += arg_len(&cmd[i]); //add the lenght of the word after redir
	return (i); 
}


char	*get_arg(char **cmd)
{
	int	len;
	char *out;

	while (**cmd == ' ')
		(*cmd)++;
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
	char	*arg;
	int	i;

	out = new_arg_redir();
	if (!out)
		return ((void *)return_msg("Couldn't allocate memory", 2, 0));
	i = 0;
	while (*cmd)
	{
		arg = get_arg(&cmd);
		if (!arg || ((arg[0] == '<' || arg[0] == '>')		//if args is NULL
			&& !ft_lstadd_back_content(&out->redir, arg))	//or push failed to redir
			|| (arg[0] != '<' && arg[0] != '>'				//or push failed to args
			&& !ft_lstadd_back_content(&out->args, arg)))
		{
			free_arg_redir(out);
			return ((void *)return_msg("Couldn't allocate memory", 2, 0));
		}
	}
	return (out);
}

t_list *split_arg_redir(char **cmd)
{
	t_list *out;
	t_arg_redir *new_cmd;

	out = NULL;
	while (*cmd)
	{
		new_cmd = get_cmd(*cmd);
		if (!new_cmd || !ft_lstadd_back_content(&out, new_cmd))
		{
			ft_lstfree(out, free_arg_redir);
			return ((void *)return_msg("Couldn't allocate memory", 2, 0)); //[ ] check this msg
		}
		cmd++;
	}
	return (out);
}

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

int main()
{
	char *cmd[] =
	{
		"hola que tal >>\"nacho es feo\"",
		"Como estás hoy \"<\" <<$variable<>   outfile",
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
	ft_lstfree(result, free_arg_redir);
	exit (0);
}