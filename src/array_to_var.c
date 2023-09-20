/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_to_var.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oscar <oscar@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 22:44:56 by oscar             #+#    #+#             */
/*   Updated: 2023/09/20 22:46:43 by oscar            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//TODO this lil function
t_list	*array_to_var(char **envp)
{
	int	len;
	int	i;
	int j;
	int aux;
	char	*name;
	char	*content;

	i = 0;
	while (envp[i])
	{
		j = 0;
		len = 0;
		while (envp[i][j] != '=')
		{
			len++;
			j++;
		}
		name = malloc(sizeof(char) * (len + 1));
		ft_strlcpy(name, envp[i], len + 1);
		j++;
		aux = j;
		len = 0;
		while (envp[i][j] != '\0')
		{
			len++;
			j++;
		}
		content = malloc(sizeof(char) * (len + 1));
		ft_strlcpy(content, &envp[i][aux], len + 1);
		set_variable(&(global->env), name, content);
		i++;
	}
	return (0);
}