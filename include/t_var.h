/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_var.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:20:58 by orudek            #+#    #+#             */
/*   Updated: 2023/09/21 17:03:17 by orudek           ###   ########.fr       */
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

char	set_variable(t_list **lst, char *name, char *content);
char	*get_variable(t_list *lst, char *name);
void	unset_variable(t_list **lst, char *name);
t_list	*array_to_varlist(char **array);
char	**varlist_to_array(t_list *lst);
void	free_var(void *var);
void	print_varlist(t_list *varlist); //XXX delete this before submitting project

#endif