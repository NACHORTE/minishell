/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   return_perror.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 14:51:31 by orudek            #+#    #+#             */
/*   Updated: 2023/10/02 14:55:40 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"

size_t return_perror(char *msg, size_t ret_val)
{
	perror(msg);
	return (ret_val);
}
