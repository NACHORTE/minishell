/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 14:28:51 by orudek            #+#    #+#             */
/*   Updated: 2023/09/05 22:47:08 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*static int	ft_get_num_words(const char *s, char c)
{
	size_t	count;
	size_t	is_word;
	size_t	comma;

	comma = 0;
	is_word = 0;
	count = 0;
	while (*s)
	{
		if (*s == '"')
		{
			if (!comma)
			{
				comma = 1;
				if (!is_word)
					count++;
			}
			else if (comma)
				comma = 0;
		}
		if (is_word && *s == c)
			is_word = 0;
		else if (!is_word && *s != c)
		{
			is_word = 1;
			if (!comma)
				count++;
		}
		s++;
	}
	printf("%ld\n", count);
	return (count);
}
*/
static int	ft_get_num_words(const char *s, char c)
{
	size_t	count;
	size_t	is_word;
	size_t	flag;

	flag = 0;
	is_word = 0;
	count = 0;
	while (*s)
	{
		if (*s == '"')
			flag = !flag;
		if (!is_word && *s != c)
		{
			is_word = 1;
			count++;
		}
		else if (is_word && !flag && *s == c)
			is_word = 0;
		s++;
	}
	return (count);
}

static char	*ft_get_word(const char **s, char c)
{
	size_t	i;
	char	*out;
	size_t	len;
	int		flag;

	//skip all spaces
	while (**s == c)
		(*s)++;
	//get amount of bytes of string
	i = 0;
	len = 0;
	flag = 0;
	while ((*s)[i] && ((*s)[i] != c || flag)) // while string is not over, and character is not space outside quotation marks pair
	{
		if ((*s)[i++] == '"')
			flag = !flag;
		else //don't count "
			len++;
	}
	out = malloc(len + 1);
	if (out == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		if (*(*s) == '"')
			(*s)++;
		out[i++] = *((*s)++);
	}
	if (*(*s) == '"')
		(*s)++;
	out[i] = 0;
	return (out);
}

static char	**ft_free(char **words)
{
	size_t	i;

	i = 0;
	while (words[i])
		free(words[i++]);
	free (words);
	return (NULL);
}

char	**ft_split_args(char const *s, char c)
{
	char	**words;
	size_t	num_words;
	size_t	i;

	num_words = ft_get_num_words(s, c);
	words = malloc((sizeof(char *) * (num_words + 1)));
	if (words == NULL)
		return (NULL);
	i = 0;
	while (i < num_words)
	{
		words[i] = ft_get_word(&s, c);
		if (!words[i])
			return (ft_free(words));
		i++;
	}
	words[i] = 0;
	return (words);
}
/*
int main(int c, char **v)
{
	char *donut = "hola a\"qu  e tal guapisimo     ignacio\"   a     ";
	char **prueba;
	printf("%s\n",donut);
	prueba = ft_split(donut, ' ');
	int i = 0;
	while (prueba[i])
		printf("%s$\n", prueba[i++]);
	return 0;
}

int main()
{
    char *cadenita = "^^^1^^2a,^^^^3^^^^--h^^^^";
    char **out = ft_split(cadenita,'^');
    while(*out)
	{
        printf("%s %p$\n", *out, *out);
		out++;
	}
	printf("%p$\n",*out);
	return (0);
}*/