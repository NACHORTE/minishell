/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   return_perror.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 14:51:31 by orudek            #+#    #+#             */
/*   Updated: 2023/10/02 15:09:42 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

unsigned long	return_perror(char *msg, unsigned long ret_val)
{
	perror(msg);
	return (ret_val);
}
