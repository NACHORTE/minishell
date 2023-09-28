/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 21:25:10 by orudek            #+#    #+#             */
/*   Updated: 2023/09/28 22:13:14 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_cmd.h"

int	cmd_env(t_list *env)
{
	//NOTE leaks after using env
	//[x] Don't print variables with content=NULL
	while (env)
	{
		if (((t_var *)env->content)->content
			&& ((t_var *)env->content)->type == ENV_VAR_TYPE)
			printf("%s=%s\n", ((t_var *)env->content)->name,
				((t_var *)env->content)->content);
		env = env->next;
	}
	return (0);
}
