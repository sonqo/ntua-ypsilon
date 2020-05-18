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

#define RED "\033[31;1m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

#define BUFFER_SIZE 129

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define STREQUAL(x, y) (strncmp((x), (y), strlen(y)) == 0)

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

    int acc[n]; // child pid accumulator

    // Creating pipes
    int pd[2*n][2];
    for (int i=0; i<2*n; i++){
        if (pipe(pd[i]) != 0){
            perror("Pipe failed");
            exit(EXIT_FAILURE);
        }
    }

    // Initialing child processes
    for (int i=0; i<2*n; i+=2){
        pid_t curr = fork();
        if (curr > 0){
            acc[i/2] = curr;
        }
        else if (curr == 0){ // child process
            int val, desc;
            char buffer[BUFFER_SIZE];
            while(1){
                read(pd[i][0], &val, sizeof(int));
                snprintf(buffer, BUFFER_SIZE, BLUE"[Child %d][%d] Child received %d"WHITE"\n", i/2, getpid(), val);
                desc = write(1, buffer, strlen(buffer));
                if (desc == -1){
                    perror("Write failed");
                    exit(EXIT_FAILURE);
                }
                val++;
                sleep(5);
                write(pd[i+1][1], &val, sizeof(int));
                snprintf(buffer, BUFFER_SIZE, GREEN"[Child %d][%d] Child finished, writing back %d"WHITE"\n", i/2, getpid(), val);
                desc = write(1, buffer, strlen(buffer));
                if (desc == -1){
                    perror("Write failed");
                    exit(EXIT_FAILURE);
                }
            }
        }
        else if (curr < 0){
            perror("Forked failed");
            exit(EXIT_FAILURE);
        }
    }

    int child_counter = 0;
    int tmax = STDIN_FILENO;
    for (int i=0; i<2*n; i++){
        tmax = MAX(tmax, pd[i+1][0]);
    }

    while(1){

        int desc, maxfd,value;
        char buffer[BUFFER_SIZE];
        fd_set inset;
        FD_ZERO(&inset);
        FD_SET(STDIN_FILENO, &inset); // checking STD for input
        for (int i=0; i<2*n; i+=1){
            FD_SET(pd[i][0], &inset); //setting all child pipes for input-checking
        }

        maxfd = tmax;

        int ready_fds = select(maxfd, &inset, NULL, NULL, NULL);
        if (ready_fds <= 0){
            perror("Select failed");
            continue;
        }

        if (FD_ISSET(STDIN_FILENO, &inset)){ // STD input
            char buffer[BUFFER_SIZE];
            int n_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
            buffer[n_read] = '\0';

            if (n_read > 0 && buffer[n_read-1] == '\n'){ // discard new line character
                buffer[n_read-1] = '\0';
            }

            if (STREQUAL(buffer, "help")){
                snprintf(buffer, BUFFER_SIZE, CYAN"Type a number to send a jod to a child"WHITE"\n");
                desc = write(1, buffer, strlen(buffer));
                if (desc == -1){
                    perror("Write failed");
                    exit(EXIT_FAILURE);
                }
            }
            else if (STREQUAL(buffer, "exit")){
                for(int i=0; i<n; i++){
                    desc = kill(acc[i], SIGTERM);
                    if (desc < 0){
                        perror("Killed failed");
                        exit(EXIT_FAILURE);
                    }
                }
                snprintf(buffer, BUFFER_SIZE, RED"All children terminated"WHITE"\n");
                desc = write(1, buffer, strlen(buffer));
                if (desc == -1){
                    perror("Write failed");
                    exit(EXIT_FAILURE);
                }
                exit(0);
            }
            else{
                int dflag = 0;
                int length = strlen(buffer); // checking if first argument is integer
                for (int i=0; i<length; i++){
                    if (!isdigit(buffer[i])){
                        dflag = 1;
                    }
                }
                if (dflag == 0){
                    value = strtol(buffer, NULL, 10);
                    if (flag == 0){
                        snprintf(buffer, BUFFER_SIZE, MAGENTA"[Parent] Assigned %d to child %d"WHITE"\n", value, child_counter);
                        desc = write(1, buffer, strlen(buffer));
                        if (desc == -1){
                            perror("Write failed");
                            exit(EXIT_FAILURE);
                        }
                        write(pd[2*child_counter][1], &value, sizeof(int));
                        child_counter++;
                        if (child_counter == n){
                            child_counter = 0;
                        }
                    }
                    else{
                        int random;
                        random = rand() % n;
                        snprintf(buffer, BUFFER_SIZE, MAGENTA"[Parent] Assigned %d to child %d"WHITE"\n", value, random);
                        desc = write(1, buffer, strlen(buffer));
                        if (desc == -1){
                            perror("Write failed");
                            exit(EXIT_FAILURE);
                        }
                        write(pd[2*random][1], &value, sizeof(int));
                    }
                }
                else{
                    printf(CYAN"Type a number to send a jod to a child"WHITE"\n");
                }
            }
        }

        for (int i=1; i<2*n+1; i+=2){
            if (FD_ISSET(pd[i][0], &inset)) {
                int val;
                read(pd[i][0], &val, sizeof(int));
                snprintf(buffer, BUFFER_SIZE, YELLOW"[Parent] Received result from child %d --> %d"WHITE"\n", i/2, val);
                desc = write(1, buffer, strlen(buffer));
                if (desc == -1){
                    perror("Write failed");
                    exit(EXIT_FAILURE);
                }
            }
        }

    }

    return 0;
}