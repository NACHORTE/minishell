/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 14:28:51 by orudek            #+#    #+#             */
/*   Updated: 2023/09/05 18:41:57 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_get_num_words(const char *s, char c)
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
	return (count);
}

static char	*ft_get_word(const char **s, char c)
{
	size_t	i;
	char	*out;
	size_t	len;

	len = 0;
	i = 0;
	while (**s == c)
		(*s)++;
	while ((*s)[len] && (*s)[len] != c)
		len++;
	out = malloc(len + 1);
	if (out == NULL)
		return (NULL);
	while (i < len)
		out[i++] = *((*s)++);
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

char	**ft_split(char const *s, char c)
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