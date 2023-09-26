#include "libft.h"
#include "t_var.h"
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

t_list	*parse(char	*input, t_list *local, t_list *env);

// Text attributes
#define RESET "\x1b[0m"      // Reset all attributes to default
#define BOLD "\x1b[1m"       // Bold or increased intensity
#define NORMAL "\x1B[22m"	 // Normal width text
#define DIM "\x1b[2m"        // Dim (decreased intensity)
#define UNDERLINE "\x1b[4m"  // Underline
#define BLINK "\x1b[5m"      // Blink
#define REVERSE "\x1b[7m"    // Reverse (swap foreground and background colors)
#define HIDDEN "\x1b[8m"     // Hidden (not widely supported)
// Text colors
#define BLACK "\x1b[30m"     	// Black text
#define RED "\x1b[31m"       	// Red text
#define GREEN "\x1b[32m"     	// Green text
#define YELLOW "\x1b[33m"    	// Yellow text
#define BLUE "\x1b[34m"      	// Blue text
#define MAGENTA "\x1b[35m"   	// Magenta text
#define CYAN "\x1b[36m"      	// Cyan text
#define WHITE "\x1b[37m"     	// White text
#define ORANGE "\x1b[38;5;208m"	// Orange text
#define GREY "\x1B[90m"			// Grey text
#define LIME "\x1B[92m"			// Lime text
#define PINK "\033[38;5;225m"   // Pink text
// Background colors
#define BG_BLACK "\x1b[40m"  // Black background
#define BG_RED "\x1b[41m"    // Red background
#define BG_GREEN "\x1b[42m"  // Green background
#define BG_YELLOW "\x1b[43m" // Yellow background
#define BG_BLUE "\x1b[44m"   // Blue background
#define BG_MAGENTA "\x1b[45m" // Magenta background
#define BG_CYAN "\x1b[46m"   // Cyan background
#define BG_WHITE "\x1b[47m"  // White background


int test_number = 1;
int	flags = 0;
#define PRINT_ALL_FLAG_NAME "-all"
#define PRINT_ALL_FLAG 0x1
#define PRINT_ERR_FLAG_NAME "-err"
#define PRINT_ERR_FLAG 0x10

#define compare_result(result,expected) \
	if (same_result(result, expected)) \
	{\
		printf(GREEN BOLD"%d.OK"RESET"\t"GREY"test:[%s]"RESET"\n",test_number,input);\
		if (flags & PRINT_ALL_FLAG)\
		{\
			printf(BLUE BOLD"\tExpected:\n"CYAN DIM);\
			print_list(expected);\
			printf(NORMAL GREEN BOLD"\tResult:\n"LIME DIM);\
			print_list(result);\
			printf(RESET NORMAL); \
		}\
	}\
	else\
	{\
		printf(RED BOLD"%d.KO"RESET"\t"GREY"test:[%s]"RESET"\n",test_number,input);\
		if (flags & PRINT_ALL_FLAG || flags & PRINT_ERR_FLAG)\
		{\
			printf(BLUE BOLD"\tExpected:\n"CYAN DIM);\
			print_list(expected);\
			printf(NORMAL GREEN BOLD"\tResult:\n"LIME DIM);\
			print_list(result);\
			printf(RESET NORMAL); \
		}\
	}
int	array_cmp(char **array1, char **array2)
{
	int i = 0;
	while(array1[i] && array2[i])
	{
		if (ft_strcmp(array1[i],array2[i]))
			return (0);
		i++;
	}
	if (!array1[i] && array2[i])
		return (0);
	if (array1[i] && !array2[i])
		return (0);
	return (1);
}

int	same_result(t_list *result, t_list *expected)
{
	if (ft_lstsize(result) != ft_lstsize(expected))
		return (0);
	while(result)
	{
		if (!array_cmp(((char **)result->content), ((char **)expected->content)))
			return (0);
		result = result->next;
		expected = expected->next;
	}
	return (1);
}

void print_list(t_list *list)
{
	int j = 0;
	if (!list)
		printf("\t\tcmd[0]: (NULL)\n");
	for (t_list *aux = list; aux; aux = aux->next)
	{
		printf("\t\tcmd[%d]:\n", j);
		int i;
		for (i = 0; ((char **)aux->content)[i]; i++)
			printf("\t\t\t%d.[%s]\n",i ,((char **)aux->content)[i]);
		printf("\t\t\t%d.[%s]\n",i ,((char **)aux->content)[i]);
		j++;
	}
}

void print_error(t_list *expected, t_list *result)
{

}

void	exec_test(char *input, t_list *expected, t_list *local, t_list *env)
{
	int pid = fork();
	if (pid == -1)
	{
		perror("Fork_failed");
		return ;
	}
	if (pid == 0)
	{
		int fd_NULL = open("/dev/null", O_WRONLY);
		if (fd_NULL == -1)
		{
			perror("/dev/null");
			exit(1);
		}
		if (!dup2(fd_NULL, 2))
		{
			perror("dup2");
			exit (1);
		}
		t_list *result = parse(input, local, env);
		compare_result(result, expected)
		ft_lstfree(result, ft_array_free);
		exit (0);
	}
	int status;
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status))
		printf(RED BOLD"%d.A"RESET"\t"GREY"test:[%s]"RESET"\n",test_number,input);
	test_number++;
}

//TESTS
////////////////////////////////////////////////////////////////////

void	test_NULL_input()
{
// INPUT VARIABLES
	//set input string
	char *input = NULL;
	//set expected result with no mallocs so its easier to check
	t_list *expected = NULL;
	//set local and env variables
	t_list *local = NULL;
	t_list *env = NULL;
// EXECUTE TEST (DON'T TOUCH)
	exec_test(input, expected, local, env);
}
void	test_empty_cmd()
{
// INPUT VARIABLES
	//set input string
	char *input = "";
	//set expected result with no mallocs so its easier to check
	char *aux[]= {"",NULL};
	t_list expected[1] = {{aux, NULL}};
	//set local and env variables
	t_list *local = NULL;
	t_list *env = NULL;
// EXECUTE TEST (DON'T TOUCH)
	exec_test(input, expected, local, env);
}

void	test_1arg_1cmd()
{
// INPUT VARIABLES
	//set input string
	char *input = "cat";
	//set expected result with no mallocs so its easier to check
	char *aux[]= {"cat",NULL};
	t_list expected[1] = {{aux, NULL}};
	//set local and env variables
	t_list *local = NULL;
	t_list *env = NULL;
// EXECUTE TEST (DON'T TOUCH)
	exec_test(input, expected, local, env);
}

void	test_1pipe()
{
// INPUT VARIABLES
	//set input string
	char *input = "cat Makefile | ls -la";
	//set expected result with no mallocs so its easier to check
	char *aux[]= {"cat", "Makefile", NULL};
	char *aux2[]= {"ls", "-la", NULL};
	t_list expected[2] = {{aux, &expected[1]}, {aux2, NULL}};
	//set local and env variables
	t_list *local = NULL;
	t_list *env = NULL;
// EXECUTE TEST (DON'T TOUCH)
	exec_test(input, expected, local, env);
}

void	set_flags(int argc, char **argv)
{
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i],PRINT_ALL_FLAG_NAME))
			flags |= PRINT_ALL_FLAG;
		if (!strcmp(argv[i],PRINT_ERR_FLAG_NAME))
			flags |= PRINT_ERR_FLAG;
	}
}

int main(int argc, char **argv, char **envp)
{
	set_flags(argc, argv);
	printf(BOLD UNDERLINE MAGENTA"TESTER: PARSE"RESET PINK" (use -all and -err for results)"RESET"\n");
	test_NULL_input();
	test_empty_cmd();
	test_1arg_1cmd();
	test_1pipe();
}
//gcc -fsanitize=leak -Iinclude -Ilibft/include src/parse.c src/expand_variables.c src/split* test/test_parse.c src/print_varlist.c src/array_to_varlist.c src/set_variable.c src/free_var.c libft/libft.a -o parse_test
