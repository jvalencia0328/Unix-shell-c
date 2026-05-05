#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>

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
        char *input_file = NULL;

        // missing output/input file flag
        bool missing_output_file = false;
        bool missing_input_file = false;

        // append true or false
        bool append_mode = false;

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
                    break;
                }
            }
            else if (strcmp(args[i], ">>") == 0)
            {
                printf("Output append redirection");
                if (args[i + 1] != NULL)
                {
                    append_mode = true;
                    output_file = args[i + 1];
                    args[i] = NULL;
                    break;
                }
                else
                {
                    fprintf(stderr, "Error: no output file specified\n");
                    missing_output_file = true;
                    break;
                }
            }
            else if (strcmp(args[i], "<") == 0)
            {
                printf("Input redirection detected\n");
                if (args[i + 1] != NULL)
                {
                    input_file = args[i + 1];
                    args[i] = NULL;
                    break;
                }
                else
                {
                    fprintf(stderr, "Error: no input file specified\n");
                    missing_input_file = true;
                    break;
                }
            }
        }
        if (missing_output_file)
            continue;
        if (missing_input_file)
            continue;

        // input redirection

        // for a new process
        pid_t p = fork();

        if (p == 0)
        {
            int fd;
            if (input_file)
            {
                fd = (open)(input_file, O_RDONLY, 0644);
                if (fd == -1)
                {
                    perror("Error: failed to create input file");
                    exit(1);
                }
                else
                {
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                }
            }
            if (output_file != NULL)
            {
                if (append_mode)
                {
                    fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);

                    if (fd == -1)
                    {
                        perror("Error: fialed to create output file");
                        exit(1);
                    }
                    else
                    {
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                    }
                }
                else
                {
                    fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (fd == -1)
                    {
                        perror("Error: failed to create output file");
                        exit(1);
                    }
                    else
                    {
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                    }
                }
            }
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