/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 16:37:59 by iortega-          #+#    #+#             */
/*   Updated: 2023/10/01 22:27:08 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	refresh_lvl(t_command *global)
{
	int	lvl;
	int	status;
	t_var	*shell_lvl;

	status = get_variable(global->varlist, "SHLVL", &shell_lvl);
	if (status == -1)
	{
		printf("Error reading env\n");
		exit (1);
	}
	if (status == 0)
	{
		shell_lvl = new_var("SHLVL", "1", ENV_VAR);
		if (!shell_lvl)
		{
			printf("Unable to set new SHLVL\n");
			exit (1);
		}
	}
	if (status == 1)
	{
		lvl = ft_atoi(shell_lvl->content) + 1;
		free(shell_lvl->content);
		shell_lvl->content = ft_itoa(lvl);
		if (!shell_lvl->content)
		{
			printf("Unable to set new SHLVL\n");
			exit (1);
		}
	}
	if (!set_variable(&(global->varlist), shell_lvl->name, shell_lvl->content, ENV_VAR))
	{
		printf("Unable to set new SHLVL\n");
		exit (1);
	}
	free_var(shell_lvl);
}

int	main(int argc, char **argv, char **envp)
{
	t_command	global;

	if (argc != 1)
		return (1);
	(void)argv;
	global.varlist = array_to_varlist(envp);
	global.last_status = 0;
	refresh_lvl(&global);
	refresh_status(&global);
	signal(SIGINT, &new_line);
	read_input(global);
	rl_clear_history();
	return (0);
}
