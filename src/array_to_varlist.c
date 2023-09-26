/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_to_varlist.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 22:44:56 by oscar             #+#    #+#             */
/*   Updated: 2023/09/25 21:53:47 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_var.h"

/*	str_find_char:
		Searches through a string and returns the first occurrence of the
		character "c" from the input.
	Parameters:
		str: string from which to search.
		c: character that will be searched
	Return:
		Returns the index where the first occurence of "c" is found in "str"
	Code:
		Just iterates the whole string and, if finds the character, returns the
		index.
*/
static int	str_find_char(char *str, char c)
{
	int	i;

	i = -1;
	while (str[++i])
		if (str[i] == c)
			return (i);
	return (i);
}

/*	split_var:
		Takes the incoming string, that is expected to have name=content
		format, and splits it into two strings containing the name and format
		separately.
		To check if the format is correct, only the existence of '=' in the
		string is checked, so name and content may be empty strings.
	Input:
		str: string with expected format name=content
	Return:
		An array containing two strings (the array is not NULL-terminated), the
		index 0 contains the name, and the index 1 the content of the input
		string.
	Code:
		First, allocates two string pointers, returning NULL if fails.
		Then, searches for a '=' character in the string. If non is found,
		returns NULL. Otherwise, the index of the character is retrieved from
		the function, and used to allocate the memory for each string.
		Both allocations are checked to save lines of code. If one fails, the
		function is exited returning NULL.
		Finally, copies each string from the input to the new strings. In the
		case of the content string, the string is offset to the position after
		the '=' character.
*/
static char	split_var(t_var *var, char *str)
{
	int		pos;
	size_t	len;

	pos = str_find_char(str, '=');
	len = ft_strlen(str);
	var->name = malloc (pos + 1);
	if (pos < len)
		var->content = malloc (len - pos);
	else
		var->content = NULL;
	if (!var->name || (len != pos && !var->content))
	{
		if (var->name)
			free(var->name);
		if (var->content)
			free(var->content);
		return (0);
	}
	ft_strlcpy(var->name, str, pos + 1);
	if (var->content)
		ft_strlcpy(var->content, ft_strrchr(str, '=') + 1, len - pos);
	return (1);
}

/*	array_to_varlist:
		Converts an array of strings formated as name=content to a list where
		each element contains a t_var struct with the name and content
		separated.
		If the format is incorrect, the string will be skipped and not added to
		the list.
		The list created can be freed with ft_lstfree(list, array_free);
	Parameters:
		array: array of strings formated as name=content that will be converted
			into a list.
	Return:
		Returns a list of t_vars that contain the name and the content of each
			string from "array". The strings that don't have the correct format
			are not saved.
	Code:
		First, if array is NULL then returns NULL.
		Then iterates through all the array, splitting each string into name
		and content. Before checking if the split was successful, the array
		pointer gets advanced to the next string to shorten the code.
		If there is an error splitting the string. It skips to the next
		string.
		The split string is saved into the list with set_variable, and if
		something goes wrong, it frees everything and returns NULL.
		Add the end, returns the created list.
*/
t_list	*array_to_varlist(char **array)
{
	t_var	var;
	t_list	*out;
	int		i;

	if (!array)
		return (NULL);
	out = NULL;
	i = -1;
	while (array[++i])
	{
		if (!split_var(&var, array[i]))
			continue ;
		if (!set_variable(&out, var.name, var.content))
		{
			ft_lstfree(out, free_var);
			free(var.name);
			free(var.content);
			return (NULL);
		}
		free(var.name);
		free(var.content);
	}
	return (out);
}

/*int main(int c, char **v, char **e)
{
	if (v[c - 1])
		c += 1;
	t_list *env = array_to_varlist(e);
	set_variable(&env, "NULLVAR", NULL);
	set_variable(&env, "NOT NULL", "\0");
	print_varlist(env);
	ft_lstfree(env, free_var);
	return (0);
}*/
// gcc -Iinclude -Ilibft/include src/array_to_varlist.c src/set_variable.c src/free_var.c src/print_varlist.c libft/libft.a -fsanitize=leak