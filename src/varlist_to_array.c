/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   varlist_to_array.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 22:46:57 by oscar             #+#    #+#             */
/*   Updated: 2023/09/21 20:59:23 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_var.h"

//no lo hago con strjoin porque no quiero hacer 3 malloc
static char	*var_to_str(t_var *var)
{
	char	*out;
	int		len[2];

	len[0] = ft_strlen(var->name);
	len[1] = ft_strlen(var->content);
	out = malloc(sizeof(char) * (len[0] + len[1] + 2)); //name + "=" + content + "\0"
	ft_strlcpy(out, var->name, len[0] + 1);
	ft_strlcpy(out + len[0], "=", 2);
	ft_strlcpy(out + len[0] + 1, var->content, len[1] + 1);
	return (out);
}

char	**varlist_to_array(t_list *lst)
{
	char	**out;
	int		i;
	int		len;

	if (!lst)
		return (NULL);
	len = ft_lstsize(lst);
	out = malloc(sizeof(char *) * (len + 1));
	if (!out)
		return (NULL);
	i = -1;
	while (++i < len)
	{
		out[i] = var_to_str((t_var *)lst->content);
		if (!out[i])
		{
			ft_array_free(out);
			return (NULL);
		}
		lst = lst->next;
	}
	out[i] = 0;
	return (out);
}
/*
#define BLACK "\x1b[30m"     	// Black text
#define RED "\x1b[31m"       	// Red text
#define GREEN "\x1b[32m"     	// Green text
#define YELLOW "\x1b[33m"    	// Yellow text
#define BLUE "\x1b[34m"      	// Blue text
#define MAGENTA "\x1b[35m"   	// Magenta text
#define CYAN "\x1b[36m"      	// Cyan text
#define WHITE "\x1b[37m"     	// White text
#define ORANGE "\x1b[38;5;208m"	// Orange text
#define RESET "\x1b[0m"      // Reset all attributes to default
#pragma GCC diagnostic ignored "-Wunused-parameter"
int main(int c, char **v, char **e)
{
	t_list *env = array_to_varlist(e);
	print_varlist(env);
	char **array = varlist_to_array(env);
	for (int i = 0; array[i]; i++)
		printf(BLUE"array["CYAN"%d"BLUE"]:\t"RESET"%s\n", i, array[i]);
	return (0);
}
gcc -Iinclude -Ilibft/include -Wall -Wextra -Werror src/array_to_varlist.c src/free_var.c src/set_variable.c src/varlist_to_array.c src/print_varlist.c libft/libft.a
*/
