/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orudek <orudek@student.42madrid.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 13:58:18 by orudek            #+#    #+#             */
/*   Updated: 2023/10/06 13:26:30 by orudek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "libft.h"
# include "error.h"

typedef struct s_arg_redir
{
	t_list	*args;
	t_list	*redir;
}	t_arg_redir;

typedef struct s_cmd
{
	char	**args;
	int		infile;
	int		outfile;
}	t_cmd;

t_list		*parse(char	*input, t_list *varlist);
char		**split_pipe(char *s);
t_list		*split_arg_redir(char **cmd);
t_arg_redir	*new_arg_redir(void);
void		free_arg_redir(void *arg_redir);
int			expand_variables(t_list *cmd, t_list *varlist);
char		*get_expanded_str(char *str, t_list *varlist);
t_list		*str_to_arg(char *str);
t_list		*str_to_redir(char *str);
int			format_variables(t_list *cmd);
t_list		*cmd_redir(t_list *pipes);
void		free_cmd(void *cmd);

#endif