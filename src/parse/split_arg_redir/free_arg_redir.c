/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_arg_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 22:20:59 by orudek            #+#    #+#             */
/*   Updated: 2023/10/06 13:14:45 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

void	free_arg_redir(void *arg_redir)
{
	t_arg_redir	*aux;

	if (!arg_redir)
		return ;
	aux = (t_arg_redir *)arg_redir;
	ft_lstfree(aux->args, free);
	ft_lstfree(aux->redir, free);
	free(arg_redir);
}
