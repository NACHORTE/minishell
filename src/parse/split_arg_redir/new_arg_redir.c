/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_arg_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 22:20:08 by orudek            #+#    #+#             */
/*   Updated: 2023/10/06 13:21:21 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

t_arg_redir	*new_arg_redir(void)
{
	t_arg_redir	*out;

	out = malloc(sizeof(t_arg_redir));
	if (!out)
		return (NULL);
	out->args = NULL;
	out->redir = NULL;
	return (out);
}
