/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   varlist_to_array.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 22:46:57 by oscar             #+#    #+#             */
/*   Updated: 2023/09/24 15:09:01 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_var.h"

//no lo hago con strjoin porque no quiero hacer 3 malloc
//[ ] write this description
static int	var_to_str(t_var *var, char **str, int ignore_null)
{
	char	*out;
	int		len[2];

	if (!var || (ignore_null && !var->content))
		return (0);
	len[0] = ft_strlen(var->name);
	len[1] = ft_strlen(var->content); //if content is null len is 0
	out = malloc(sizeof(char) * (len[0] + (var->content != 0) + len[1] + 1)); //name + "=" + content + "\0"
	if (!out)
		return (0);
	ft_strlcpy(out, var->name, len[0] + 1);
	if (var->content != 0)
	{
		ft_strlcpy(out + len[0], "=", 2);
		ft_strlcpy(out + len[0] + 1, var->content, len[1] + 1);
	}
	*str = out;
	return (1);
}

int	varlist_len(t_list *lst, int ignore_null)
{
	t_var	*var;
	int		count;

	count = 0;
	while (lst)
	{
		var = (t_var *)lst->content;
		if (var && (ignore_null == 0 || var->content != 0))
			count++;
		lst = lst->next;
	}
	return (count);
}

/*	varlist_to_array:
		converts a list of variables to string array where each string has the
		format name=content.
		It is posible to choose between saving the variables which content is
		NULL or not in the output string with the ignore_null variable. Setting
		it to 1, doesn't save variables with null content, and setting it to 0
		saves them only storing the name without adding '=' character.
		If NULLs are ignored, reverting back to a t_list from the returned array
		will lose all those variables.
	Parameters:
		lst: list containing each variable in a t_var struct that has a name
			and a content.
		ignore_null: Option for choosing whether to store NULLs (0) or not (1).
	Return:
		An array of strings containing all the variables in the "lst" variable.
		Each string in the array has the format "name=content" unless the
		content of a variable is NULL in which case, if NULLs are not ignored,
		the format for that string is "name".
	Code:
		[ ] Write this description
*/

static char	**save_vars(char **out, t_list *lst, int len, int ignore_null)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if (var_to_str((t_var *)lst->content, &out[i], ignore_null))
		{
			if (!out[i])
			{
				ft_array_free(out);
				return (NULL);
			}
			i++;
		}
		lst = lst->next;
	}
	out[i] = 0;
	return (out);
}

char	**varlist_to_array(t_list *lst, int ignore_null)
{
	char	**out;
	int		len;

	if (!lst)
		return (NULL);
	len = varlist_len(lst, ignore_null);
	out = malloc(sizeof(char *) * (len + 1));
	if (!out)
		return (NULL);
	return (save_vars(out, lst, len, ignore_null));
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
	//t_list *env = array_to_varlist(e);
	t_var *hello = malloc(sizeof(t_var));
	hello->name = ft_strdup("nombre");
	hello->content = ft_strdup("contenido");
	t_var *hello2 = malloc(sizeof(t_var));
	hello2->name = ft_strdup("nombre2");
	hello2->content = ft_strdup("");
	t_var *hello3 = malloc(sizeof(t_var));
	hello3->name = ft_strdup("nombre3");
	hello3->content = NULL;
	t_list *env = ft_lstnew(hello);
	ft_lstadd_back(&env,ft_lstnew(NULL));
	ft_lstadd_back(&env,ft_lstnew(hello2));
	ft_lstadd_back(&env,ft_lstnew(hello3));
	print_varlist(env);
	char **array = varlist_to_array(env, 1);
	for (int i = 0; array && array[i]; i++)
		printf(BLUE"array["CYAN"%d"BLUE"]:\t"RESET"%s\n", i, array[i]);
	exit (0);
}
//gcc -Iinclude -Ilibft/include -fsanitize=leak -Wall -Wextra -Werror src/array_to_varlist.c src/free_var.c src/set_variable.c src/varlist_to_array.c src/print_varlist.c libft/libft.a
*/
