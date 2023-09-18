
t_list	*read_new_line(char *message, t_list *local, t_list *env)
{
	t_list	*output;
	char	*input

	input = readline(message);
	if (!input)
		exit (1);
	if (ft_strlen(input) == 0)
	{
		free(input);
		return (NULL);
	}
	add_history(input);
	if (!check_closed_quotes(input))
	{
		free(input);
		return (NULL);
	}
	output = parse(input, local, env);
	free (input);
	return (output);
}

unsigned long	*free_all(t_struct global, unsigned long return_val)
{
	if (is_freeable)
		muchos frees
	return (return_val);
}

void	init(t_struct global)
{
	fd van a -1;
	pointers a NULL;
}

MUCHAS FUNCIONES DE FREEAR COSAS Y QUE SE PONGAN EN NULL

void exec_command(t_list *cmd, t_list **env, t_list **local)
{
	Comprobar que el primero es builtin
		If lo es se ejecuta sin fork
		Else se ejecuta con fork
	No hacer el pipe si solo hay un comando
	Hacer while para crear todos los forks y pipes
	Hacer el último fork
	while (wait(&status) != -1) al final
	Guardar ultimo status en $?
}

int main(int argc, char **argv, char **envp)
{
	t_struct global;

	init(global);
	envp_to_list(&global.env, envp);
	while(1)
	{
		signal();
		global.cmd = read_new_line(INPUT_MSG, global.env, global.local);
		if(!global.cmd) //If read_new_line fails, skip the loop
			continue ;
		exec_command(global.cmd, &global.env, &global.local) //If this function fails, np
		ft_lstfree(global.cmd, ft_array_free);
		global.cmd = NULL;
	}
	return (free_all(global,0));
}
