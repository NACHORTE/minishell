/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 13:58:18 by orudek            #+#    #+#             */
/*   Updated: 2023/10/05 19:14:50 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "libft.h"
# include "error.h"

typedef struct s_arg_redir
{
	t_list *args;
	t_list *redir;
} t_arg_redir;

t_arg_redir *new_arg_redir(void);
void		free_arg_redir(void *arg_redir);

typedef struct s_cmd
{
	char **args;
	int	infile;
	int outfile;
} t_cmd;

char	**split_pipe(char *s);
int		expand_variables(t_list *cmd, t_list *varlist);
char	**split_args(char const *s, char c);
t_list	*parse(char	*input, t_list *varlist);
t_list	*cmd_redir(t_list *pipes);
void	free_cmd(void *cmd);
t_list	*split_arg_redir(char **cmd);

#endif