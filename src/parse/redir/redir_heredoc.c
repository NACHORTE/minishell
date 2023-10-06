/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 13:09:10 by iortega-          #+#    #+#             */
/*   Updated: 2023/10/06 15:40:34 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include <fcntl.h>
#include "minishell.h"

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
	int		i;
	int		fd;
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
