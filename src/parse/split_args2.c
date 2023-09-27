/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 23:37:26 by orudek            #+#    #+#             */
/*   Updated: 2023/09/17 16:19:10 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*	dup_str:
		Auxiliary function used in "get_word"
		Copies the word found in string "s" into string "out" ignoring
		quotations that surround a string and ending when finding the delimiter
		character or '<' '>'.
	Parameters:
		out: string where the word will be copied
		s: string where the word will be searched
		len: length of the word that will be copied
		c: delimiter character between words
	Return:
		The returned result is the "out" variable
	Code:
		Iterates until the string is over or the required characters have been
		written in "out".
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
		With every successful iteration the character is written in "out".
*/
void	dup_str(char *out, const char *s, int len)
{
	int	i;
	int	state;

	state = 0;
	i = 0;
	s--;
	while (i < len && *++s)
	{
		if ((state == 0 && *s == '\'') || (state == 0 && *s == '"'))
		{
			state = (*s == '\'') * 2 + (*s == '"');
			continue ;
		}
		if ((state == 1 && *s == '"') || (state == 2 && *s == '\''))
		{
			state = 0;
			continue ;
		}
		out[i++] = *s;
	}
	out[i] = 0;
}

/*	dup_redirection:
		Auxiliary function used in "get_word"
		Copies the word found in string "s" into string "out" using the format
		of:
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
		out: string where the word will be copied
		s: string where the word will be searched
		len: length of the word that will be copied
		c: delimiter character between words
	Return:
		The returned result is the "out" variable
	Code:
		The code advances the "s" string whenever is needed and to iterate
		"out", the variable "i" is used.
		First the first value of "s" is copied in out as it is supposed to be
		'<' or '>'.
		Then if the second character is the same as the first it is stored too
		If the second character is '<' or '>' but diferent than the first, its
		skipped.
		Then "s" is advanced skipping all the delimiter characters
		Finally the rest of the word is stored using the "dup_str" function.
*/
void	dup_redirection(char *out, const char *s, int len, char c)
{
	int	i;

	out[0] = *s++;
	i = 1;
	if (*s == out[0])
		out[i++] = *s++;
	else if (*s == '<' || *s == '>')
		s++;
	while (*s == c)
		s++;
	(void)dup_str(&out[i], s, len - i);
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
void	get_str_len(const char *s, char c, int *len, int *i)
{
	int	state;

	state = 0;
	*len = 0;
	*i = -1;
	while (s[++(*i)]
		&& (state || (s[*i] != c && s[*i] != '<' && s[*i] != '>')))
	{
		if (state == 0 && (s[*i] == '\'' || s[*i] == '"'))
		{
			state = (s[*i] == '\'') * 2 + (s[*i] == '"');
			continue ;
		}
		if ((state == 1 && s[*i] == '"') || (state == 2 && s[*i] == '\''))
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
void	get_redirection_len(const char *s, char c, int *len, int *i)
{
	int	str_len;
	int	str_i;

	*i = 1;
	*len = 1;
	if (s[1] == s[0])
	{
		(*i)++;
		(*len)++;
	}
	else if (s[1] == '<' || s[1] == '>')
		(*i)++;
	while (s[*i] == c)
		(*i)++;
	(void)get_str_len(&s[*i], c, &str_len, &str_i);
	*i += str_i;
	*len += str_len;
}
