/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 21:26:28 by orudek            #+#    #+#             */
/*   Updated: 2023/09/27 22:39:48 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	cmd_pwd()
{
	char	*dir;

	//NOTE: FIRST try saving in buffer with #include <limits.h> and buffer[PATH_MAX];
	// if i doesn't work do the malloc;
	dir = getcwd(NULL, 0);
	if (dir == NULL)
	{
		printf("ERROR: PWD\n"); //[ ]: define.h
		return (1);
	}
	printf("%s\n", dir);
	free (dir);
	return (0);
}
