#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// main shell loop:
int main()
{
    while (1)
    {
        // print prompt
        printf("my_shell> ");

        // read user input
        char input[100];

        // read user input
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        // remove newline character:
        input[strcspn(input, "\n")] = 0;

        // if user just pressed Enter, skip and show prompt again
        if (strlen(input) == 0)
            continue;

        // process the exit command

        if (strcmp(input, "exit") == 0)
            break;

        // tokenize the input

        char delimiter[] = " ";

        char *token = strtok(input, delimiter);

        // array of string pointers to store command and arguments
        char *args[100];

        int i = 0;
        while (token != NULL)
        {
            // print the token
            printf("Token: %s\n", token);
            // Store the token in the args array
            args[i] = token;

            i++;
            token = strtok(NULL, delimiter);
        }

        // Null-terminate the args array
        args[i] = NULL;

        // for a new process
        pid_t p = fork();

        if (p == 0)
        {
            // child process
            printf("Child process\n");
        }
        else if (p > 0)
        {
            // parent process
            printf("Parent process\n");
        }
        else
        {
            // failed fork
            printf("Fork failed\n");
        }
    }
}