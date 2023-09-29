/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 21:25:10 by orudek            #+#    #+#             */
/*   Updated: 2023/09/29 13:48:40 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_cmd.h"

int	cmd_env(t_list *varlist)
{
	while (varlist)
	{
		if (((t_var *)varlist->content)->content
			&& ((t_var *)varlist->content)->type == ENV_VAR)
			printf("%s=%s\n", ((t_var *)varlist->content)->name,
				((t_var *)varlist->content)->content);
		varlist = varlist->next;
	}
	return (0);
}
