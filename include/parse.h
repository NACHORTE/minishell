/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 13:58:18 by orudek            #+#    #+#             */
/*   Updated: 2023/10/06 13:12:20 by iortega-         ###   ########.fr       */
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
int			format_variables(t_list *cmd);
t_list		*cmd_redir(t_list *pipes);
void		free_cmd(void *cmd);
int			check_restdin_heree(t_list *redir);
void		redirect_streamss(t_cmd *out, t_list *redir);

#endif