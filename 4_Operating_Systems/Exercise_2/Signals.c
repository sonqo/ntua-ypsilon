#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define BUFFER_SIZE 128
#define MAXIMUM_TIME 100

int acc_pid[25];
int global_argc, global_prc, global_counter, z;

static void info_FatherHandler(){
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "\n[Father Process %d] Will ask current values from all active children processes:\n", getpid());
    int desc = write(1, buffer, strlen(buffer));
    if (desc < 0){
        perror("Write failed.");
    }
    for (int i = 0; i < global_argc; i++){
        int desc = kill(acc_pid[i], SIGUSR1);
        if (desc < 0){
            perror("Kill failed.\n");
        }
    }
}

static void info_ChildHandler(){
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "\n[Child Process %d: %d] Value: %d!\n", global_counter, getpid(), z);
    int desc = write(1, buffer, strlen(buffer));
    if (desc < 0){
        perror("Write failed.\n");
    }
}

static void echo_FatherHandler(){
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "\n[Process %d] Echo!\n", getpid());
    int desc = write(1, buffer, strlen(buffer));
    if (desc < 0){
        perror("Write failed.\n");
    }
}

static void echo_ChildHandler(){
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "\n[Child Process %d] Echo!\n", getpid());
    int desc = write(1, buffer, strlen(buffer));
    if (desc < 0){
        perror("Write failed.\n");
    }
}

static void alarm_ChildHandler(){
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "[Child Process %d: %d] Time expired! Final value: %d\n", global_counter, getpid(), z);
    int desc = write(1, buffer, strlen(buffer));
    if (desc < 0){
        perror("Write failed.\n");
    }
    desc = kill(getpid(), SIGTERM); // Auto-terminate
    if (desc < 0){
        perror("Kill failed.\n");
    }
}

static void terminate_FatherHandler(){
    char buffer[BUFFER_SIZE];
    write(1, "\n", 1);
    for (int i = 1; i < global_argc+1; i++){
        snprintf(buffer, BUFFER_SIZE, "[Father Process %d] Will terminate child process %d: %d\n", getpid(), i, acc_pid[i-1]);
        int desc = write(1, buffer, strlen(buffer));
        if (desc < 0){
            perror("Write failed.\n");
        }
        desc = kill(acc_pid[i-1], SIGTERM);
        if (desc < 0){
            perror("Write failed.\n");
        }
    }
}

// Starting point
int main(int argc, char **argv){

    pid_t curr;
    int desc, acc_tim[argc-1];
    
    global_argc = argc-1;
    global_prc = global_argc;

    struct sigaction echoFather, echoChild, infoFather, infoChild, alarmChild, terminateFather, terminateChild;
    
    echoFather.sa_handler = echo_FatherHandler;
    echoChild.sa_handler = echo_ChildHandler;
    
    infoFather.sa_handler = info_FatherHandler;
    infoChild.sa_handler = info_ChildHandler;

    alarmChild.sa_handler = alarm_ChildHandler;

    terminateFather.sa_handler = terminate_FatherHandler;

    // Parse command-line arguments
    for (int i = 0; i < argc-1; i++){
        acc_tim[i] = atoi(argv[i+1]);
    }
    
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "Maximum execution time of children is set to %d secs:\n\n", MAXIMUM_TIME);
    write(1, buffer, strlen(buffer));

    // Initializing fork processes
    for (global_counter = 1; global_counter < argc; global_counter++){
        curr = fork();
        if (curr == 0){ // Child process
            sigaction(SIGUSR1, &infoChild, NULL);
            sigaction(SIGUSR2, &echoChild, NULL);
            sigaction(SIGALRM, &alarmChild, NULL);
            sigaction(SIGINT, &terminateChild, NULL);
            snprintf(buffer, BUFFER_SIZE, "[Child process %d: %d] Was created and will pause!\n", global_counter, getpid());
            desc = write(1, buffer, strlen(buffer));
            if (desc == -1){
                perror("Write failed\n");
                exit(EXIT_FAILURE);
            }
            int desc = kill(getpid(), SIGSTOP);
            if (desc < 0){
                perror("Kill failed\n");
            }
            alarm(MAXIMUM_TIME);
            snprintf(buffer, BUFFER_SIZE, "[Child process %d: %d] Is starting!\n", global_counter, getpid());
            desc = write(1, buffer, strlen(buffer));
            if (desc == -1){
                perror("Write failed\n");
            }
            z = 0;
            while (1){
                z++;
                sleep(acc_tim[global_counter-1]);
            }
            break;
        }
        else if (curr > 0){ // Father keeps track of child PIDs
            acc_pid[global_counter-1] = curr;
        }
        else{
            perror("Fork failed\n");
        }
    }

    if (curr > 0){ // Father initializes SIGCONT
        sleep(1);
        sigaction(SIGUSR1, &infoFather, NULL);
        sigaction(SIGUSR2, &echoFather, NULL);
        sigaction(SIGINT, &terminateFather, NULL);
        sigaction(SIGTERM, &terminateFather, NULL);
        for (int i = 0; i < argc-1; i++){
            int desc = kill(acc_pid[i], SIGCONT);
            if (desc < 0){
                perror("Kill failed\n");
            }
        }
        write(1, "\n", 1);
        int status;
        pid_t wpid;
        while ((wpid = wait(&status)) > 0) ;
        while (errno == 4){ // EINTR error catched
            while ((wpid = wait(&status)) > 0) ;
        }
    }

    return 0;
}