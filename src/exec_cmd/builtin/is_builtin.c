/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 14:20:10 by orudek            #+#    #+#             */
/*   Updated: 2023/09/29 14:22:12 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_builtin(char *cmd)
{
	if (!cmd || !*cmd)
		return (0);
	if (!ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "export"))
		return (1);
	return (0);
}
