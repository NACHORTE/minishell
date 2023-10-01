/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 16:37:59 by iortega-          #+#    #+#             */
/*   Updated: 2023/10/01 16:38:43 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	new_line(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	refresh_status(t_command *global)
{
	char	*content;

	content = ft_itoa(global->last_status);
	set_variable(&(global->varlist), "?", content, DEFAULT_VAR);
	free(content);
}

void	read_input(t_command global)
{
	char	*input;

	while (1)
	{
		input = readline("\033[36mminishell >> \033[0m");
		if (input == NULL)
		{
			printf("exit\n");
			exit(1);
		}
		add_history(input);
		if (check_closed_quotes(input))
		{
			global.cmds = parse(input, global.varlist);
			if (is_command(&global))
			{
				global.last_status = exec_cmd(global.cmds, &global.varlist);
				refresh_status(&global);
			}
			if (global.cmds)
				ft_lstfree(global.cmds, ft_array_free);
		}
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_command	global;

	if (argc != 1)
		return (1);
	(void)argv;
	global.varlist = array_to_varlist(envp);
	global.last_status = 0;
	refresh_status(&global);
	signal(SIGINT, &new_line);
	read_input(global);
	rl_clear_history();
	return (0);
}
