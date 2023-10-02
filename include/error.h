/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 14:52:15 by orudek            #+#    #+#             */
/*   Updated: 2023/10/02 14:54:56 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

#include "libft.h"

size_t return_perror(char *msg, size_t ret_val);
size_t return_msg(char *msg, int fd, size_t ret_val);

#endif