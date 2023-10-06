/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 14:03:19 by orudek            #+#    #+#             */
/*   Updated: 2023/10/06 14:03:41 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

void	free_cmd(void *cmd)
{
	t_cmd	*aux;

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
