/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 13:58:18 by orudek            #+#    #+#             */
/*   Updated: 2023/09/29 14:00:11 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "libft.h"

char	**split_pipe(char *s);
char	*expand_variables(char *str, t_list *varlist);
char	**split_args(char const *s, char c);
t_list	*parse(char	*input, t_list *varlist);

#endif