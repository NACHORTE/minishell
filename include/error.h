/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 14:52:15 by orudek            #+#    #+#             */
/*   Updated: 2023/10/02 15:09:35 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

unsigned long	return_perror(char *msg, unsigned long ret_val);
unsigned long	return_msg(char *msg, int fd, unsigned long ret_val);

#endif