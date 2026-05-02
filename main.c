#include <stdio.h>
#include <string.h>

//main shell loop:
int main() {
    while(1){
        //print prompt
        printf("my_shell> ");
        
        
        
        //read user input
        char input[100];
        
        //read user input
       if (fgets(input, sizeof(input), stdin) == NULL){
        break;
       }

       //remove newline character:
       input[strcspn(input, "\n")] = 0;

       // if user just pressed Enter, skip and show prompt again
       if(strlen(input) == 0)
            continue;

        
    }
}