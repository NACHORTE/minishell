/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 14:28:51 by orudek            #+#    #+#             */
/*   Updated: 2023/09/09 23:12:44 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*dup_str(char *out, const char *s, int len, char c)
{
	int	i;
	int	state;

	state = 0;
	i = 0;
	s--;
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
		out[i++] = *s;
	}
	return (out);
}

static char *dup_redirection(char *out, const char *s, int len, char c)
{
	int i;
	int j;

	out[0] = *s++;
	i = 1;
	if (*s == out[0])
		out[i++] = *s++;
	else if (*s == '<' || *s == '>')
		*s++;
	while (*s == c)
		s++;
	if (*s == '<' || *s == '>')
		return (out);
	(void)dup_str(&out[i], s, len - i, c);
	return (out);
}

/*	dup_word:
		Auxiliary function used in "get_word"
		Creates a new string of size "len", and stores the next word found in
		the passed string "s".
	Parameters:
		s: String where to search the word.
		len: length of the word that will be created
		c: delimiter character used to separate words
	Return:
		Returns the a new string containing the next word found in "s".
		The word is formatted ignoring certain characters depending on the 
		type of word (string or redirection)
	Code:
		First allocates the memory of the word
*/
static char	*dup_word(const char *s, int len, char c)
{
	char	*out;
	int		i;

	out = malloc(len + 1);
	if (out == NULL)
		return (NULL);
	if (*s == '<' || *s == '>')
		return (dup_redirection(out, s, len, c));
	else
		return (dup_str(out, s, len, c));
}

/*	get_str_len:
		Auxiliary function used in "get_word" and "get_num_words"
		Counts the length of the word ignoring quotes that surround strings,
		and ending when a delimiter or '<' '>' is found outside quotes.
		If quotes are besides more non delimiter characters, it still counts as
		the same word.
	Parameters:
		s: string from which to count the length of the word.
		c: character used as the delimiter between words.
		len: reference to the variable where the length of the word will be
			stored (length of the characters without the ignored ones)
		i: reference to the variable where the total length of the word will be
			store (lenght counting ignored characters)
	Return:
		The return is done in "len" and "i" variables.
	Code:
		Iterates the "s" string using "i" until its the end of the string, or a
		delimiter character or '<' '>' is found outside quotes.
		The loop works with states:
			-state 0: Means the read characters are outside quotes
			-state 1: Means the read characters are inside "" quotes
			-state 2: Means the read characters are inside '' quotes
		(state 1 and 2 are different because there can be opposite quotes
		inside)
		When in state 0, if any quote is found the state will change. The
		assignation is done like that to shorten the code. if s[i]=' the result
		is 2 and if s[i]=" the result is 1
		When in either state 1 or 2, if the same quotation type is found, state
		goes back to 0.
		In both state transitions, the iteration is skipped so the surrounding
		quotation characters don't get counted in the length.
		With every successful iteration, the length of the word is increased by
		one.
*/
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

/*	get_redirection_len:
		Auxiliary function used in "get_word" and "get_num_words"
		Counts the length of the next word of the string assuming it will have
		this format:
			<[<][spaces][name]
		The first character is supossed to be '<' or '>'
		The second character can be a '<' or '>', but it will only be counted
		in the length if it is the same as the first one.
		After the delimiter there can be [0-n] delimiter characters that will
		not be counted in the length.
		The name is gonna be treated a string, allowing "" or '', and
		ending if it finds '<' or '>'. The quotes surrounding a string won't be
		counted in the length.
	Parameters:
		s: string from which to count the length of the word.
		c: character used as the delimiter between words.
		len: reference to the variable where the length of the word will be
			stored (length of the characters without the ignored ones)
		i: reference to the variable where the total length of the word will be
			store (lenght counting ignored characters)
	Return:
		The return is done in "len" and "i" variables.
	Code:
		First sets "len" and "i" to 1 because the first character is '<'.
		Then if the second one is the same as the first, adds 1 to both
		variables.
		If the second variable is a '<' but not the same type, don't count it
		in "len" but still increase "i"
		Afterwards, increase i with as many delimiter characters there are
		until a word or '\0' is found.
		Then call the "get_str_len" function to get the lenght of the string
		add the length returned from the function to "len" and "i" to get the
		total length.
*/
static void	get_redirection_len(const char *s, char c, int *len, int *i)
{
	int str_len;
	int str_i;

	*i = 1;
	*len = 1;
	if (s[1] == s[0])
	{
		(*i)++;
		(*len)++;
	}
	else if (s[1] == '<' || s[1] == '>')
		(*i)++;
	while(s[*i] == c)
		(*i)++;
	(void)get_str_len(&s[*i], c, &str_len, &str_i);
	*i += str_i;
	*len += str_len;
}

/*	get_word:
		Auxiliary function used in "split_args".
		Searches in a string the next word found following the description
		in "split_args" of a word.
		It also moves the pointer to the position after the end of the word.
	Parameters:
		s: reference to the string where the word will be searched.
		c: delimiter for splitting words
	Return:
		Returns a new string with the word found saved.
	Code:
		First it skips all the delimiter characters.
		Then it calls to different functions of calculating the length of the
		word depending of whether the first character found is <,> or not.
		Then calls the function "dup_word" that creates the string and stores
		the word inside.
		Finally it advances the pointer of "s" to the byte after the end of the
		string and returns the word.
*/
static char	*get_word(const char **s, char c)
{
	int		len;
	int		i;
	char	*word;

	while (**s == c)
		(*s)++;
	if (**s == '>' || **s == '<')
		get_redirection_len(*s, c, &len, &i);
	else
		get_str_len(*s, c, &len, &i);
	word = dup_word(*s, len, c);
	if (!word)
		return (NULL);
	*s += i;
	return (word);
}

/*	get_num_words:
		Auxiliary function used in "split_args".
		Counts the amount of words that are in the given string.
		Uses the rules of "split_args" function to identify a word
	Parameters:
		s: the string from which to count the words
		c: the delimiter used to separate words
	Return:
		Returns the amount of words found in the string
	Code:
		Loops until the string is over.
		First, it skips all delimiter characters on each iteration
		Then it must be on the beginning of a word, so it chooses
		whether it is a <,> type or a string type and calls the functions.
		"get_redirection_len" and "get_str_len" return the length of the word
		without all the skipped characters which is saved in "len" (like 
		spaces after < or "" surrounding a string), and also how many bytes
		uses the word, which is saved in "i" (counting all spaces and "")
		Then checks if "i" is greater than 0, to avoid counting the last
		iteration if the string ends with the delimiter character and adds
		1 to the count of words if the condition is true
		finally, it advances the "s" variable "i" positions to skip to the next
		character after the end of the word
*/
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
		if (*s == '>' || *s == '<')
			get_redirection_len(s, c, &len, &i);
		else
			get_str_len(s, c, &len, &i);
		if (i)
			count++;
		s += i;
	}
	return (count);
}

/*	split_args:
		Takes an input shell and splits it in words by the selected character
		like a shell would.
		A word is considered as follows:
			-If there are no "",'',<,> a word are all the characters between
				the selected character.
				(eg. [ hello nacho] -> [hello] [nacho])
			-If there are "", '' a word is what is inside them. Also, other
				strings atached to them are in the same word.
					eg. [ hello" nacho"] -> [hello nacho]
					eg. [ hello" '>nacho'"] -> [hello '>nacho']
			-If there are <,> it will save the < and the next one if it is the
				same as the first. Then it will skip all the delimitators until
				finding a word. If it finds nothing or another <, it just saves
				the < or <<.
					eg. [ <infile cat>> outfile] -> [<infile] [cat] [>>outfile]
					eg. [ <>infile > > cat] -> [<infile] [>] [>cat]
	Parameters:
		s: the string that will be splitted
		c: the delimiter used to split the string (usually space)
	Return:
		returns a double array where every array[i] is a string.
		Both the array and the strings inside are null terminated.
	Code:
		first it counts the amount of words using the description of earlier.
		Then allocates string pointers for the words.
		Then enters a loop until all the words are saved. Calls the get_word
		function to get the string and advance the s pointer to the next
		character after the end of the word.
		If there is a fail, array free will deallocate all the current words
		Writes a null to the last position of the array to indicate its end
		Returns the new words array.
*/
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

/*
int main(int c, char **v)
{
	char **s = split_args(v[1], ' ');
	for (int i = 0; s[i]; i++)
		printf("str[%d]=\'%s\'\n",i,s[i]);
	return (1);
}*/
