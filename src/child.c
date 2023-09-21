/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iortega- <iortega-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:29:18 by oscar             #+#    #+#             */
/*   Updated: 2023/09/21 12:37:28 by iortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*  child:
        Executes a command that can have redirections.
        If the command doesn't have a command redirection for a stream, uses
        the corresponding one from the function parameters.
    Parameters:
        infile: File descriptor of the file from which the command will read
            the data
        outfile: File descriptor of the file to which the command will write
			the data
		cmd: Array of strings that each contains an argument of the command
			(eg. [cat] [>outfile] [Makefile])
		env: Array of strings containing all the environment variables (not
			the local ones).
	Return:
		NOTHING! it must exit the program at the end of this function
	Todo:
		In a separate function
		[ ] REDIR: Create all files from the redirections in cmd
		[ ] REDIR: <<Here Doc
		[ ] REDIR: >>append mode
		[ ] REDIR: If no redirection in the cmd, redirect to infile/outfile
		[ ]	CHECK_BUILTINS
		[ ] ACCESS: "./" "../" "/"
		[ ] ACCESS: [OPTIONAL] if no path, add ./ to cmd if it doesn't have it
		[ ] ACCESS: if cmd[i]="\0", don't crash and print "'': cmd not found"
		[ ] perror with cmd name after excve
		[ ] use nice names :)
		[ ] algo de signals nose
		[ ] norminette
*/

void	check_restdin(char **input)
{
	int	i;
	int	j;
	int	flag;
	int	fd;

	i = 0;
	flag = 0;
	/*while (input[i])
	{
		printf("%s\n", input[i]);
		i++;
	}
	i = 0;*/
	while (input[i])   //if we have < " " file we get flag 1 and check file, else if we have <file we check file without "<"
	{
		j = 0;
		if (input[i][j] == '<')
		{
			if (fd != 0)
				close (fd);
			j++;
			fd = open(&input[i][j], O_RDONLY);
			if (fd < 0)
			{
				perror(&input[i][j]);
				exit(1);
			}
			flag = 1;
		}
		i++;
	}
	if (flag)
	{
		dup2(fd, 0);
		close(fd);
	}
}

int	check_restdout(char **input)
{
	int	i;
	int	j;
	int	flag;
	int	fd;
	char	*redi;

	i = 0;
	flag = 0;
	fd = 0;
	/*while (input[i])
	{
		printf("%s\n", input[i]);
		i++;
	}
	i=0;*/
	while (input[i])   //if we have > " " file we get flag 1 and check file, else if we have >file we check file without ">"
	{
		j = 0;
		if (input[i][j] == '>')
		{
			if (fd != 0)
				close (fd);
			j++;
			flag = 1;
			redi = &input[i][j];
			if (input[i][j] == '>')
			{
				j++;
				fd = open(&input[i][j], O_WRONLY | O_APPEND, 0666);
			}
			else
				fd = open(&input[i][j], O_WRONLY | O_TRUNC | O_CREAT, 0666);
		if (fd < 0)
		{
			perror(&input[i][j]);
			return (-1);
		}
			//break;
		}
		i++;
	}
	if (flag)
	{
		dup2(fd, 1);
		close(fd);
	}
	return (fd);
}

void    child(int infile, int outfile, char **cmd, char **env)
{
	//makes the needed dup2, creates the files if there are multiple output redirections
	// and opens the correct file.
    redirect_streams(infile, outfile, cmd);
    //removes the redirections from the command returning a new char **array
    cmd_parsed = parse_cmd();
    //gets the full path of the command
    cmd_path = get_cmd_path();
    excve(cmd_path, cmd_parsed, env);
	perror(cmd_parsed[0]);
	exit(errno); //NOTE maybe just exit 1 is OK
}