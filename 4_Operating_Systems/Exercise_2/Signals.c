#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define BUFFER_SIZE 64

static void echo_handler(){
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "[Child Process %d] Echo!\n", getpid());
    write(1, buffer, strlen(buffer));
}

// Starting point
int main(int argc, char **argv){

    struct sigaction action;
    action.sa_handler = echo_handler;
    // action.sa_flags = SA_RESTART;
    sigaction(SIGUSR2, &action, NULL);

    int acc_tim[argc-1];
    int acc_pid[argc-1];

    // Parse command-line arguments
    for (int i = 0; i < argc-1; i++){
        acc_tim[i] = atoi(argv[i+1]);
    }

    pid_t curr;
    pid_t father = getpid();
    printf("%d\n", father);

    // Initializing fork processes
    for (int i = 1; i < argc; i++){
        curr = fork();
        if (curr == 0){ // Child process
            printf("[Child process %d: %d] Was created and will pause!\n", i, getpid());
            raise(SIGSTOP);
            printf("[Child process %d: %d] Is starting!\n", i, getpid());
            int counter = 0;
            while (1){
                counter++;
                sleep(acc_tim[i]);
            }
            break;
        }
        else if (curr == father){ // Father keeps track of child PIDs
            acc_pid[i-1] = curr;
        }
    }

    if (curr > father){ // Father initializes SIGCONT
        sleep(1);
        for (int i = 0; i < argc-1; i++){
            kill(acc_pid[i], SIGCONT);
        }
        wait(NULL);
        while (errno == 4){
            wait(NULL);
        }
    }

    // for (int i = 0; i < argc-1; i++){
    //     printf("%d ", acc_pid[i]);
    // }
    // printf("\n");

    return 0;
}