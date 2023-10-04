/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_arg_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 22:20:59 by orudek            #+#    #+#             */
/*   Updated: 2023/10/03 23:22:46 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

void	free_arg_redir(void *arg_redir)
{
	t_arg_redir *aux;

	aux = (t_arg_redir *)arg_redir;
	ft_lstfree(aux->args, free);
	ft_lstfree(aux->redir, free);
	free(arg_redir);
}
