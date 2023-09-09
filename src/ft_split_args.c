/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 14:28:51 by orudek            #+#    #+#             */
/*   Updated: 2023/09/09 17:41:02 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
static void	get_redirection_len(const char *s, char c, int *len, int *i);
static void	get_str_len(const char *s, char c, int *len, int *i);
static char	*dup_str(char *out, const char *s, int len, char c);

static int	get_num_words(const char *s, char c)
{
	int	count;
	int	len;
	int	i;

	while (*s)
	{
		i = 0;
		while (*s == c)
			s++;
		//get the lenght of the word found
		if (*s == '>' || *s == '<')
			get_redirection_len(s, c, &len, &i);
		else
			get_str_len(s, c, &len, &i);
		if (i)
			count++;
		s += i;
	}
	printf("number of words: %d\n",count);
	return (count);
}

static char *dup_redirection(char *out, const char *s, int len, char c)
{
	int i;

	//save all the spaces and << < > >> until a letter is found
	//this parse allows "<< < > hello" to be considered a word
	i = 0;
	while(*s == c || *s == '<' || *s == '>')
	{
		if (*s != c)
			out[i++] = *s;
		s++;
	}
	(void)dup_str(&out[i], s, len - i, c);
	return (out);
}

static char	*dup_str(char *out, const char *s, int len, char c)
{
	int	i;
	int	state;

	state = 0;
	i = 0;
	s--;  //go back 1 character so i can do ++s in the while condition
	printf("\tDUP_STR: s= ||%s||\n", s+1);
	while(i < len && *++s)
	{
		if (state == 0 && *s == '\'' || state == 0 && *s == '"')
		{
			state = (*s == '\'') * 2 + (*s == '"');
			continue ;
		}
		if (state == 1 && *s == '"' || state == 2 && *s == '\'')
		{
			state = 0;
			continue ;
		}
		printf("\tDUP_STR: state=%d, *s: %c\n",state,*s);
		out[i++] = *s;
	}
	return (out);
}

static char	*dup_word(const char *s, int len, char c)
{
	char	*out;
	int		i;

	//allocate memory for the new string
	out = malloc(len + 1);
	if (out == NULL)
		return (NULL);
	if (*s == '<' || *s == '>')
	{
		printf("DUP_WORD: calling DUP_REDIRECTION\n");
		return (dup_redirection(out, s, len, c));
	}
	else
	{
		printf("DUP_WORD: calling DUP_STR\n");
		return (dup_str(out, s, len, c));
	}
}

static void	get_redirection_len(const char *s, char c, int *len, int *i)
{
	int str_len;
	int str_i;

	*i = 0;
	*len = 0;
	while(s[*i] == c || s[*i] == '<' || s[*i] == '>')
	{
		if (s[*i] != c)
			(*len)++;
		(*i)++;
	}
	(void)get_str_len(&s[*i], c, &str_len, &str_i);
	*i += str_i;
	*len += str_len;
}

static void	get_str_len(const char *s, char c, int *len, int *i)
{
	int state = 0;

	state = 0;
	*len = 0;
	*i = -1;
	while(s[++(*i)] && (state || (s[*i] != c && s[*i] != '<' && s[*i] != '>')))
	{
		if (state == 0 && (s[*i] == '\'' || s[*i] == '"'))
		{
			//if s == ' -> state = 1 else state = 2
			state = (s[*i] == '\'') * 2 + (s[*i] == '"');
			continue ;
		}
		if (state == 1 && s[*i] == '"' || state == 2 && s[*i] == '\'')
		{
			state = 0;
			continue ;
		}
		(*len)++;
	}
}

static char	*get_word(const char **s, char c)
{
	int		len;
	int		i;
	char	*word;

	//skip all spaces
	printf("GETWORD: string before skipping spaces: ||%s||\n",*s);
	while (**s == c)
		(*s)++;
	printf("GETWORD: string after skipping spaces: ||%s||\n",*s);
	//get the lenght of the word found
	if (**s == '>' || **s == '<')
	{
		get_redirection_len(*s, c, &len, &i);
		printf("GETWORD: string len from GET_REDIRECTION_LEN: len:%d, i:%d\n", len, i);
	}
	else
	{
		get_str_len(*s, c, &len, &i);
		printf("GETWORD: string len from GET_STR_LEN: len:%d, i:%d\n", len, i);
	}
	//get a new string with the word saved
	word = dup_word(*s, len, c);
	printf("GETWORD: word saved as: ||%s||\n",word);
	if (!word)
		return (NULL);
	//advance the pointer to the end of the word
	*s += i;
	printf("GETWORD: string after getting advanced %d spaces : ||%s||\n", i, *s);
	//return a new string with the word (ignoring "", '', or spaces after < >)
	return (word);
}

char	**split_args(char const *s, char c)
{
	char	**words;
	size_t	num_words;
	size_t	i;

	num_words = get_num_words(s, c);
	words = malloc((sizeof(char *) * (num_words + 1)));
	if (words == NULL)
		return (NULL);
	i = 0;
	while (i < num_words)
	{
		words[i] = get_word(&s, c);
		if (!words[i])
			return ((char **)ft_array_free(words));
		i++;
	}
	words[i] = 0;
	return (words);
}

int main(int c, char **v)
{
	char **prueba;
	if (c != 2)
		return 1;
	printf("printf:\"%s\"\n",v[1]);
	prueba = split_args(v[1], ' ');
	int i = 0;
	while (prueba[i])
		printf("%s$\n", prueba[i++]);
	return 0;
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