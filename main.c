#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>

// main shell loop:
int main()
{
    while (1)
    {
        // print prompt
        printf("Prompt> ");

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

        bool too_many_args = false;
        while (token != NULL)
        {
            if (i >= 99)
            {
                fprintf(stderr, "Error: too many arguments\n");
                too_many_args = true;
                break;
            }
            else

            {
                // Store the token in the args array
                args[i] = token;
            }
            i++;
            token = strtok(NULL, delimiter);
        }
        if (too_many_args)
            continue;

        // Null-terminate the args array
        args[i] = NULL;

        // build cd command
        if (strcmp(args[0], "cd") == 0)
        {
            if (args[1] == NULL)
            {
                // go to home directory
                chdir(getenv("HOME"));
            }
            else
            {
                int result = chdir(args[1]);

                if (result != 0)
                {
                    perror("cd: failed to change directory\n");
                }
            }
            continue;
        }

        /// handle output redirection
        char *output_file = NULL;

        // missing output file flag
        bool missing_output_file = false;
        for (int i = 0; args[i] != NULL; i++)
        {
            if (strcmp(args[i], ">") == 0)
            {
                printf("Output redirection detected\n");
                if (args[i + 1] != NULL)
                {
                    output_file = args[i + 1];
                    args[i] = NULL;
                    break;
                }
                else
                {
                    fprintf(stderr, "Error: no output file specified\n");
                    missing_output_file = true;
                }
            }
        }
        if (missing_output_file)
            continue;

        // for a new process
        pid_t p = fork();

        if (p == 0)
        {
            // child process
            execvp(args[0], args);
            // if fail print error
            perror("execvp failed");
            exit(1);
        }
        else if (p > 0)
        {
            // parent process
            wait(NULL);
        }
        else
        {
            // failed fork
            perror("Fork failed\n");
        }
    }
}