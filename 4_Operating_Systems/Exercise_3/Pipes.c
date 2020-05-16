#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define BUFFER_SIZE 128
#define MAXIMUM_TIME 100

#define STREQUAL(x, y) (strncmp((x), (y), strlen(y)) == 0)

char buffer[BUFFER_SIZE];

void correctUsage() {
    printf("Usage:%s  <nChildren> [--round-robin] [--random]\n", "%s");
    exit(EXIT_FAILURE);
}

// Starting point
int main(int argc, char **argv){
    
    int flag, child_num;

    // Command line parsing
    if (argc > 3){
        correctUsage();
    }
    else if (argc == 3){
        if (STREQUAL(argv[2], "--round-robin")){
            int length = strlen(argv[1]); // checking if first argument is integer
            for (int i=0; i<length; i++){
                if (isdigit(argv[1][i])){
                    flag = 0; //round-robin method
                    child_num = strtoll(argv[1], NULL, 10);
                }
                else{
                    correctUsage();
                }
            }
        }
        else if (STREQUAL(argv[2], "--random")){
            int length = strlen(argv[1]); // checking if first argument is integer
            for (int i=0; i<length; i++){
                if (isdigit(argv[1][i])){
                    flag = 1; //random method
                    child_num = strtoll(argv[1], NULL, 10);
                }
                else{
                    correctUsage();
                }
            }
        }
        else{
            correctUsage();
        }
    }
    else if (argc == 2){
        int length = strlen(argv[1]); // checking if first argument is integer
        for (int i=0; i<length; i++){
            if (isdigit(argv[1][i])){
                flag = 0; //round-robin method
                child_num = strtoll(argv[1], NULL, 10);
            }
            else{
                correctUsage();
            }
        }
    }
    else{
        correctUsage();
    }

    // printf("%d %d", flag, child_num);

    return 0;
}