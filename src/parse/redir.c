/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:51:13 by iortega-          #+#    #+#             */
/*   Updated: 2023/10/04 12:46:23 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include <fcntl.h>
#include "minishell.h"

static void	check_doublestdin(char *character, int *fd)
{
	if (*fd != 0)
		close (*fd);
	if (*character == '<')
	{
		if (*fd != 0)
			close (*fd);
		*fd = 0;
	}
	else
	{
		*fd = open(character, O_RDONLY);
		if (*fd < 0)
			perror(character);
	}
}

static int	check_restdin(t_list *redir)
{
	int	i;
	int	fd;
	char	*str;
	
	i = 0;
	fd = 0;
	while (fd >= 0 && redir)
	{
		str = (char *)redir->content;
		i = 0;
		if (str[i] == '<')
			check_doublestdin(&str[i + 1], &fd);
		redir = redir->next;
	}
	return (fd);
}

static void	check_doublestdout(char *str, int *fd)
{
	int	j;

	j = 0;
	if (str[j] == '>')
	{
		if (*fd != 0)
			close (*fd);
		j++;
		if (str[j] == '>')
		{
			j++;
			*fd = open(&str[j], O_WRONLY | O_APPEND | O_CREAT, 0644);
			if (*fd < 0)
				perror(&str[j]);
		}
		else
			*fd = open(&str[j], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (*fd < 0)
			perror(&str[j]);
	}
}

static int	check_restdout(t_list *redir)
{
	int	fd;
	char	*str;

	fd = 0;
	while (fd >= 0 && redir)
	{
		str = (char *)redir->content;
		check_doublestdout(str, &fd);
		redir = redir->next;
	}
	/*if (fd > 0)
	{
		dup2(fd, 1);
		close(fd);
	}*/
	return (fd);
}

void	redirect_streams(t_cmd *out, t_list *redir)
{
	int	fd_in;
	int	fd_out;

	fd_in = check_restdin(redir);
	fd_out = check_restdout(redir);
	if (fd_in <= -1)
	{
		out->infile = -1;
	}
	else if (fd_in > 0)
	{
		if (out->infile > 0)
			close(out->infile);
		out->infile = fd_in;
		//close(fd_in);
	}
	if (fd_out <= -1)
		out->outfile = -1;
	else if (fd_out > 0)
		out->outfile = fd_out;
	else if (fd_out == 0)
		out->outfile = 1;
}

static void	check_doublestdin_here(char *character, int *fd)
{
	if (*fd != 0)
		close (*fd);
	if (*character == '<')
	{
		*fd = here_doc(character + 1);
	}
	else
	{
		if (*fd != 0)
			close (*fd);
		*fd = 0;
	}
}

int	check_restdin_here(t_list *redir)
{
	int	i;
	int	fd;
	char	*str;

	fd = 0;
	while (fd >= 0 && redir)
	{
		str = (char *)redir->content;
		i = 0;
		if (str[i] == '<')
		{
			check_doublestdin_here(&str[i + 1], &fd);
		}
		redir = redir->next;
	}
	return (fd);
}

char	**save_args(t_list *args)
{
	char **out;
	int	i;

	i = ft_lstsize(args);
	out = malloc(sizeof(char *) * (i + 1));
	if (!out)
		return (NULL);
	out[i] = NULL;
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
		args->next;
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
		return (NULL);
	redirect_streams(out, (t_list *)cmd->redir);
	out->args = save_args((t_list *)cmd->args);
	if (!out->args)
		return (NULL);
	return (out);
}

void	free_cmd(void *cmd)
{
	t_cmd *aux;

	if (!cmd)
		return ;
	aux = (t_cmd *)cmd;
	ft_array_free(aux->args);
	if (aux->infile > 1)
		close(aux->infile);
	if (aux->outfile > 1)
		close(aux->outfile);
	free(aux);
}

t_list	*cmd_redir(t_list *pipes)
{
	t_cmd	*cmd;
	t_list	*out;
	int	fdin;

	out = NULL;
	while (pipes)
	{
		cmd = get_cmd((t_arg_redir*)pipes->content);
		if (!cmd || !ft_lstadd_back_content(&out, cmd))
		{
			ft_lstfree(cmd, free_cmd); //crear free
			return (1); //mensaje error
		}
		pipes = pipes->next;
	}
}