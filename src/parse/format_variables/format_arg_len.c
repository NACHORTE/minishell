/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_arg_len.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 13:41:56 by orudek            #+#    #+#             */
/*   Updated: 2023/10/06 13:50:37 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	format_arg_len(char *s)
{
	int	len;
	int	state;

	state = 0;
	len = 0;
	s--;
	while (*++s)
	{
		if (state == 0 && (*s == '\'' || *s == '"'))
		{
			state = (*s == '\'') * 2 + (*s == '"');
			continue ;
		}
		else if ((state == 1 && *s == '"') || (state == 2 && *s == '\''))
		{
			state = 0;
			continue ;
		}
		len++;
	}
	return (len);
}
