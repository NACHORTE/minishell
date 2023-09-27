/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_var.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:20:58 by orudek            #+#    #+#             */
/*   Updated: 2023/09/27 22:49:08 by orudek           ###   ########.fr       */
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
int		get_variable(t_list *lst, char *name, char **content);
void	unset_variable(t_list **lst, char *name);
t_list	*array_to_varlist(char **array);
char	**varlist_to_array(t_list *lst, int ignore_null);
void	free_var(void *var);
void	print_varlist(t_list *varlist); //XXX delete this before submitting project
int		is_varname_ok(const char *name);

#endif