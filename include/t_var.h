/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_var.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:20:58 by orudek            #+#    #+#             */
/*   Updated: 2023/09/11 13:48:19 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_VAR_H
# define T_VAR_H

# include "libft.h"

typedef struct s_variable
{
	char	*name;
	char	*content;
}	t_var;

//t_var *new_var(char *name, char *content);
char	set_variable(t_list **lst, t_var *var);
char	*get_variable(t_list *lst, char *name);
void	unset_variable(t_list **lst, char *name);
void	free_var(void *var);

#endif