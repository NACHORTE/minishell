/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_var.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscar <oscar@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:20:58 by orudek            #+#    #+#             */
/*   Updated: 2023/09/20 22:37:24 by oscar            ###   ########.fr       */
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
char	set_variable(t_list **lst, char *name, char *content);
char	*get_variable(t_list *lst, char *name);
void	unset_variable(t_list **lst, char *name);
t_list	*array_to_var(char **array);
char	**var_to_array(t_list *lst);
void	free_var(void *var);

#endif