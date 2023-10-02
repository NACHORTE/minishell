/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 21:26:28 by orudek            #+#    #+#             */
/*   Updated: 2023/10/02 15:14:23 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "error.h"

int	cmd_pwd(void)
{
	char	*dir;

	dir = getcwd(NULL, 0);
	if (dir == NULL)
		return (return_msg("ERROR: pwd", 2, 1));
	printf("%s\n", dir);
	free (dir);
	return (0);
}
