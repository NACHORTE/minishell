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
		printf(GREEN BOLD"%2d.OK"RESET"\t%s\t"GREY"input:[%s]"RESET"\n",test_number,name,input);\
		if (flags & PRINT_ALL_FLAG)\
		{\
			printf(BLUE BOLD"\tExpected:\n"CYAN DIM);\
			print_var(expected);\
			printf(NORMAL GREEN BOLD"\tResult:\n"LIME DIM);\
			print_var(result);\
			printf(RESET NORMAL); \
		}\
	}\
	else\
	{\
		printf(RED BOLD"%2d.KO"RESET"\t%s\t"GREY"input:[%s]"RESET"\n",test_number,name,input);\
		if (flags & PRINT_ALL_FLAG || flags & PRINT_ERR_FLAG)\
		{\
			printf(BLUE BOLD"\tExpected:\n"CYAN DIM);\
			print_var(expected);\
			printf(NORMAL GREEN BOLD"\tResult:\n"LIME DIM);\
			print_var(result);\
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

void print_var(t_list *list)
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

void	exec_test(char *input, t_list *expected, t_list *local, t_list *env, char *name)
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
		void *result = parse(input, local, env);
		compare_result(result, expected)
		//ft_lstfree((t_list *)result, ft_array_free);
		exit (0);
	}
	int status;
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status))
		printf(RED BOLD"%2d.A"RESET"\t%s\t"GREY"input:[%s]"RESET"\n",test_number,name,input);
	test_number++;
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

//TESTS
////////////////////////////////////////////////////////////////////
void	test_norminette(char *norminette_cmd)
{
	int old_stdout = dup(1);
	int old_stderr = dup(2);
	int fd_NULL = open("/dev/null",O_WRONLY);
	dup2(fd_NULL,2);
	dup2(fd_NULL,1);
	int status = WEXITSTATUS(system(norminette_cmd));
	dup2(old_stdout,1);
	dup2(old_stderr,2);
	if (status == 127)
		printf(CYAN BOLD"norminette not available\n"RESET);
	else if (status == 1)
		printf(CYAN BOLD"norminette:"RED" KO\n"RESET);
	else if (status == 0)
		printf(CYAN BOLD"norminette:"GREEN" OK\n"RESET);
	else
		printf(CYAN BOLD"norminette exit status = %d\n"RESET,status);
}

void	test_NULL_input()
{
// INPUT VARIABLES
	//set input string
	char *name = "input is NULL";
	char *input = NULL;
	//set expected result with no mallocs so its easier to check
	t_list *expected = NULL;
	//set local and env variables
	t_list *local = NULL;
	t_list *env = NULL;
// EXECUTE TEST (DON'T TOUCH)
	exec_test(input, expected, local, env, name);
}
void	test_empty_cmd()
{
// INPUT VARIABLES
	//set input string
	char *name = "input is empty string";
	char *input = "";
	//set expected result with no mallocs so its easier to check
	char *aux[]= {NULL};
	t_list expected[1] = {{aux, NULL}};
	//set local and env variables
	t_list *local = NULL;
	t_list *env = NULL;
// EXECUTE TEST (DON'T TOUCH)
	exec_test(input, expected, local, env, name);
}
void	test_1arg_1cmd()
{
// INPUT VARIABLES
	//set input string
	char *name = "1 cmd 1 arg";
	char *input = "cat";
	//set expected result with no mallocs so its easier to check
	char *aux[]= {"cat",NULL};
	t_list expected[1] = {{aux, NULL}};
	//set local and env variables
	t_list *local = NULL;
	t_list *env = NULL;
// EXECUTE TEST (DON'T TOUCH)
	exec_test(input, expected, local, env, name);
}
void	test_multi_arg_1cmd()
{
// INPUT VARIABLES
	//set input string
	char *name = "1 cmd multi arg";
	char *input = "cat Makefile outfile";
	//set expected result with no mallocs so its easier to check
	char *aux[]= {"cat", "Makefile", "outfile",NULL};
	t_list expected[1] = {{aux, NULL}};
	//set local and env variables
	t_list *local = NULL;
	t_list *env = NULL;
// EXECUTE TEST (DON'T TOUCH)
	exec_test(input, expected, local, env, name);
}
void	test_1pipe()
{
// INPUT VARIABLES
	//set input string
	char *name = "normal cmd with 1 pipe";
	char *input = "cat Makefile | ls -la";
	//set expected result with no mallocs so its easier to check
	char *aux[]= {"cat", "Makefile", NULL};
	char *aux2[]= {"ls", "-la", NULL};
	t_list expected[2] = {{aux, &expected[1]}, {aux2, NULL}};
	//set local and env variables
	t_list *local = NULL;
	t_list *env = NULL;
// EXECUTE TEST (DON'T TOUCH)
	exec_test(input, expected, local, env, name);
}
void	test_multiple_pipes_with_redir()
{
// INPUT VARIABLES
	//set input string
	char *name = "multi cmd, multi arg, multi redirs";
	char *input = "cat Makefile >>outfile <infile | <> in2  >< out2 ls -la | <<<here_doc cat";
	//set expected result with no mallocs so its easier to check
	char *aux[]= {"cat", "Makefile",">>outfile","<infile", NULL};
	char *aux2[]= {"<in2",">out2","ls", "-la", NULL};
	char *aux3[]= {"<<", "<here_doc", "cat", NULL};
	t_list expected[] = {{aux, &expected[1]}, {aux2, &expected[2]}, {aux3, NULL}};
	//set local and env variables
	t_list *local = NULL;
	t_list *env = NULL;
// EXECUTE TEST (DON'T TOUCH)
	exec_test(input, expected, local, env, name);
}
void	test_double_pipe()
{
// INPUT VARIABLES
	//set input string
	char *name = "double pipe between 2 cmd";
	char *input = "cat Makefile || command2";
	//set expected result with no mallocs so its easier to check
	char *aux[]= {"cat", "Makefile", NULL};
	char *aux2[]= {NULL};
	char *aux3[]= {"command2", NULL};
	t_list expected[] = {{aux, &expected[1]}, {aux2, &expected[2]}, {aux3, NULL}};
	//set local and env variables
	t_list *local = NULL;
	t_list *env = NULL;
// EXECUTE TEST (DON'T TOUCH)
	exec_test(input, expected, local, env, name);
}
void	test_only_1pipe()
{
// INPUT VARIABLES
	//set input string
	char *name = "only a pipe";
	char *input = "|";
	//set expected result with no mallocs so its easier to check
	char *aux[]= {NULL};
	char *aux2[]= {NULL};
	t_list expected[] = {{aux, &expected[1]}, {aux2, NULL}};
	//set local and env variables
	t_list *local = NULL;
	t_list *env = NULL;
// EXECUTE TEST (DON'T TOUCH)
	exec_test(input, expected, local, env, name);
}
void	test_expand_1dollar()
{
// INPUT VARIABLES
	//set input string
	char *name = "only a $";
	char *input = "$";
	//set expected result with no mallocs so its easier to check
	char *aux[]= {"$",NULL};
	t_list expected[] = {{aux, NULL}};
	//set local and env variables
	t_list local[] = {NULL};
	t_list env[] = {NULL};
// EXECUTE TEST (DON'T TOUCH)
	exec_test(input, expected, local, env, name);
}
void	test_expand_non_existing_var()
{
// INPUT VARIABLES
	//set input string
	char *name = "only not existing var";
	char *input = "$no_exist";
	//set expected result with no mallocs so its easier to check
	char *aux[]= {NULL};
	t_list expected[] = {{aux, NULL}};
	//set local and env variables
	t_var var[] = {{"name1","content1"},{"name2","content2"},{"name3","content3"}};
	t_list local[] = {{&var[0],&local[1]},{&var[1],&local[2]},{&var[2],NULL}};
	t_list *env = NULL;
// EXECUTE TEST (DON'T TOUCH)
	exec_test(input, expected, local, env, name);
}
void	test_env_contains_null()
{
// INPUT VARIABLES
	//set input string
	char *name = "env[0]={NULL},local=NULL";
	char *input = "$no_exist";
	//set expected result with no mallocs so its easier to check
	char *aux[]= {NULL};
	t_list expected[] = {{aux, NULL}};
	//set local and env variables
	t_var var[] = {{"name1","content1"},{"name2","content2"},{"name3","content3"}};
	t_list *local = NULL;
	t_list env[] = {NULL};
// EXECUTE TEST (DON'T TOUCH)
	exec_test(input, expected, local, env, name);
}
void	test_local_contains_null()
{
// INPUT VARIABLES
	//set input string
	char *name = "local[0]={NULL},env=NULL, try use var";
	char *input = "$no_exist";
	//set expected result with no mallocs so its easier to check
	char *aux[]= {NULL};
	t_list expected[] = {{aux, NULL}};
	//set local and env variables
	t_var var[] = {{"name1","content1"},{"name2","content2"},{"name3","content3"}};
	t_list local[] = {NULL};
	t_list *env = NULL;
// EXECUTE TEST (DON'T TOUCH)
	exec_test(input, expected, local, env, name);
}
void	test_pipe_in_var()
{
// INPUT VARIABLES
	//set input string
	char *name = "pipe=\"|\", should not split command";
	char *input = "cat Makefile $pipe grep hola";
	//set expected result with no mallocs so its easier to check
	char *aux[]= {"cat", "Makefile", "|", "grep", "hola",NULL};
	t_list expected[] = {{aux, NULL}};
	//set local and env variables
	t_var var[] = {{"name1","content1"},{"pipe","|"},{"name3","content3"}};
	t_list local[] = {{&var[0],&local[1]},{&var[1],&local[2]},{&var[2],NULL}};
	t_list *env = NULL;
// EXECUTE TEST (DON'T TOUCH)
	exec_test(input, expected, local, env, name);
}
void	test_existing_vars()
{
// INPUT VARIABLES
	//set input string
	char *name = "Multiple existing vars in env and local";
	char *input = "$name1 $name2 $name3 $name4";
	//set expected result with no mallocs so its easier to check
	char *aux[]= {"content1", "content2", "content3", "content4",NULL};
	t_list expected[] = {{aux, NULL}};
	//set local and env variables
	t_var var[] = {{"name1","content1"},{"name2","content2"},{"name3","content3"},{"name4","content4"}};
	t_list local[] = {{&var[0],&local[1]},{&var[1],NULL}};
	t_list env[] = {{&var[2], &env[1]}, {&var[3],NULL}};
// EXECUTE TEST (DON'T TOUCH)
	exec_test(input, expected, local, env, name);
}
void	test_existing_closevars()
{
// INPUT VARIABLES
	//set input string
	char *name = "Multiple existing vars in env and local close to each other";
	char *input = "$name1$name2$name3$name4";
	//set expected result with no mallocs so its easier to check
	char *aux[]= {"content1content2content3content4",NULL};
	t_list expected[] = {{aux, NULL}};
	//set local and env variables
	t_var var[] = {{"name1","content1"},{"name2","content2"},{"name3","content3"},{"name4","content4"}};
	t_list local[] = {{&var[0],&local[1]},{&var[1],NULL}};
	t_list env[] = {{&var[2], &env[1]}, {&var[3],NULL}};
// EXECUTE TEST (DON'T TOUCH)
	exec_test(input, expected, local, env, name);
}
void	test_double_dollar()
{
// INPUT VARIABLES
	//set input string
	char *name = "Double dollar";
	char *input = "$$";
	//set expected result with no mallocs so its easier to check
	char *aux[]= {"dollarcontent",NULL};
	t_list expected[] = {{aux, NULL}};
	//set local and env variables
	t_var var[] = {{"$","dollarcontent"},{"name2","content2"},{"name3","content3"},{"name4","content4"}};
	t_list local[] = {{&var[0],&local[1]},{&var[1],NULL}};
	t_list env[] = {{&var[2], &env[1]}, {&var[3],NULL}};
// EXECUTE TEST (DON'T TOUCH)
	exec_test(input, expected, local, env, name);
}
void	test_cmd_closeredir()
{
	// INPUT VARIABLES
	//set input string
	char *name = "close arg to redir";
	char *input = "cat>outfile";
	//set expected result with no mallocs so its easier to check
	char *aux[]= {"cat",">outfile",NULL};
	t_list expected[] = {{aux, NULL}};
	//set local and env variables
	t_list *local = NULL;
	t_list *env = NULL;
// EXECUTE TEST (DON'T TOUCH)
	exec_test(input, expected, local, env, name);
}
int main(int argc, char **argv, char **envp)
{
	set_flags(argc, argv);
	printf(BOLD UNDERLINE MAGENTA"TESTER: PARSE"RESET PINK" (use -all and -err for results)"RESET"\n");
	test_norminette("norminette src/parse/*");
	test_NULL_input();
	test_empty_cmd(); //maybe this test should return a NULL list.
	test_1arg_1cmd();
	test_multi_arg_1cmd();
	test_1pipe();
	test_multiple_pipes_with_redir();
	test_double_pipe();
	test_only_1pipe();
	test_expand_1dollar();
	test_expand_non_existing_var();
	test_env_contains_null();
	test_local_contains_null();
	test_pipe_in_var();
	test_existing_vars();
	test_existing_closevars();
	test_double_dollar();
	test_cmd_closeredir();
}
//gcc -fsanitize=leak -Iinclude -Ilibft/include src/parse.c src/expand_variables.c src/split* test/test_parse.c src/print_varlist.c src/array_to_varlist.c src/set_variable.c src/free_var.c libft/libft.a -o parse_test
