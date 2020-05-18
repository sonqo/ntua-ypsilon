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
#include <sys/select.h>

#define DEFAULT "\033[30;1m"
#define RED "\033[31;1m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define GRAY "\033[38;1m"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define STREQUAL(x, y) (strncmp((x), (y), strlen(y)) == 0)

char buffer[BUFFER_SIZE];

void correctUsage() {
    printf("Usage:%s  <nChildren> [--round-robin] [--random]\n", "%s");
    exit(EXIT_FAILURE);
}

// Starting point
int main(int argc, char **argv){
    
    int n, flag;

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
                    n = strtoll(argv[1], NULL, 10);
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
                    n = strtoll(argv[1], NULL, 10);
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
                n = strtoll(argv[1], NULL, 10);
            }
            else{
                correctUsage();
            }
        }
    }
    else{
        correctUsage();
    }

    // Creating pipes
    int pd[2*n][2];
    for (int i=0; i<n; i++){
        if (pipe(pd[i]) != 0){
            perror("Pipe failed");
            exit(EXIT_FAILURE);
        }
    }
    
    // Initialing father-child processes
    for (int i=0; i<n; i++){
        pid_t curr = fork();
        if (curr == 0){ // child process
            int val;
            while(1){
                close(pd[i][1]);
                close(pd[i+1][0]);
                while(read(pd[i][0], &val, sizeof(int)) < 0);
                printf(BLUE"[Child %d][%d] Child received %d!"WHITE"\n", i+1, getpid(), val);
                val++;
                sleep(5);
                write(pd[i+1][1], &val, sizeof(int));
                printf(GREEN"[Child %d][%d] Childer finished, writing back %d"WHITE"\n", i+1, getpid(), val);
            }
        }
        else if (curr >0){ // father process
            while(1){
                int maxfd;
                fd_set inset;
                FD_ZERO(&inset);
                FD_SET(STDIN_FILENO, &inset); // checking STD for input
                for (int i=0; i<n; i++){
                    FD_SET(pd[i][0], &inset); //checking all child pipes for input
                }
                maxfd = MAX(STDIN_FILENO, pd[0]) + 1;

                int ready_fds = select(maxfd, &inset, NULL, NULL, NULL);
                if (ready_fds <= 0){
                    perror("Select failed");
                    continue;
                }
                if (FD_ISSET(STDIN_FILENO, &inset)){
                    char buffer[101];
                    int n_read = read(STDIN_FILENO, buffer, 100);
                    buffer[n_read] = '\0';

                    // New-line is also read from the stream, discard it.
                    if (n_read > 0 && buffer[n_read-1] == '\n'){
                        buffer[n_read-1] = '\0';
                    }

                    printf(BLUE"Got user input: '%s'"WHITE"\n", buffer);

                    if (n_read >= 4 && strncmp(buffer, "exit", 4) == 0){
                        // user typed 'exit', kill child and exit properly
                        kill(pid, SIGTERM);                         // error checking!
                        wait(NULL);                                 // error checking!
                        exit(0);
                    }
                }
            }
        }
        else{
            perror("Forked failed");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}