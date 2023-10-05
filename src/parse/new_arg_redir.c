/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_arg_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 22:20:08 by orudek            #+#    #+#             */
/*   Updated: 2023/10/03 22:21:40 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

t_arg_redir *new_arg_redir(void)
{
	t_arg_redir *out;

	out = malloc(sizeof(t_arg_redir));
	if (!out)
		return (NULL);
	out->args = NULL;
	out->redir = NULL;
	return (out);
}
