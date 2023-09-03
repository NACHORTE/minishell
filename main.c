#include "minishell.h"

void	free_double(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	main()
{
	char	*input;
	char	**parsed;
	int		i;
	int		exit;

	exit = 1;
	while (exit)
	{
		input = readline("\033[36mminishell >> \033[0m");
		parsed = ft_split(input, ' ');
		i = 0;
		while (parsed[i])
		{
			if (!ft_strcmp(parsed[i], "exit") && i == 0)
			{
				exit = 0;
				break;
			}
			printf("%s\n", parsed[i]);
			i++;
		}
		free_double(parsed);
		free(input);
	}
	/*free_double(parsed);
	free(input);*/
	return (0);
}
