/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_var.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 13:20:58 by orudek            #+#    #+#             */
/*   Updated: 2023/10/06 12:39:51 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_VAR_H
# define T_VAR_H

# include "libft.h"

# define DEFAULT_VAR 0
# define LOCAL_VAR 1
# define ENV_VAR 2

typedef struct s_var
{
	char	*name;
	char	*content;
	int		type;
}	t_var;

t_list	*array_to_varlist(char **array);
char	**varlist_to_array(t_list *varlist, int type);

t_var	*new_var(char *name, char *content, int type);
t_var	*dup_var(t_var *var);
void	free_var(void *var);

int		set_variable(t_list **lst, char *name, char *content, int type);
int		set_variable2(t_list **lst, t_var *var);
int		get_variable(t_list *lst, char *name, t_var **var);
void	unset_variable(t_list **lst, char *name);

int		is_in_varlist(t_list *lst, char *name);
int		is_varname_ok(const char *name);

#endif