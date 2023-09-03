#include <stdio.h>

#include <readline/readline.h>
#include <readline/history.h>

int main()
{
        char *inpt;

        int i = 0;

        while ( i < 10 )
        {
                inpt = readline("Enter text: ");
                add_history(inpt);
                printf("%s", inpt);
                printf("\n");
                ++i;
        }

        return 0;

}