/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 14:28:51 by orudek            #+#    #+#             */
/*   Updated: 2023/09/16 13:52:59 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	dup_redirection(char *out, const char *s, int len, char c);
void	get_str_len(const char *s, char c, int *len, int *i);
void	get_redirection_len(const char *s, char c, int *len, int *i);

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
		Then allocates the needed memory for the word and checks again the
		first character to decide which copying function to call.
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
	word = malloc(len + 1);
	if (word == NULL)
		return (NULL);
	if (**s == '<' || **s == '>')
		dup_redirection(word, *s, len, c);
	else
		ft_memcpy(word, *s, len);
	word[len] = '\0';
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

	count = 0;
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
		printf("str[%d]=[%s]\n",i,s[i]);
	return (1);
}*/
