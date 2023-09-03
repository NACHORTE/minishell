/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_array_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 13:19:48 by orudek            #+#    #+#             */
/*   Updated: 2023/08/23 14:27:05 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_array_free(char **array)
{
	int	i;

	if (!array)
		return (0);
	i = -1;
	while (array[++i])
		free(array[i]);
	free(array);
	return (0);
}
