/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   return_msg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 14:51:25 by orudek            #+#    #+#             */
/*   Updated: 2023/10/02 15:09:39 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "unistd.h"
#include "libft.h"

unsigned long	return_msg(char *msg, int fd, unsigned long ret_val)
{
	if (msg)
	{
		write(fd, msg, ft_strlen(msg));
		write(fd, "\n", 1);
	}
	return (ret_val);
}