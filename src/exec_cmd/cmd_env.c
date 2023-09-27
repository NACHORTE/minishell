/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 21:25:10 by orudek            #+#    #+#             */
/*   Updated: 2023/09/27 21:25:22 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	cmd_env(t_list *env)
{
	//NOTE leaks after using env
	//[x] Don't print variables with content=NULL
	while (env)
	{
		if (((t_var *)env->content)->content);
			printf("%s=%s\n", ((t_var *)env->content)->name,
				((t_var *)env->content)->content);
		env = env->next;
	}
	return (0);
}
