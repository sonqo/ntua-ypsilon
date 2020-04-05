#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define BUFFER_SIZE 128
#define MAXIMUM_TIME 100

int acc_pid[25];
bool acc_bool[25];
int global_argc, global_prc, global_counter, z;

static void info_FatherHandler(){
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "\n[Father Process %d] Will ask current values from all active children processes:\n", getpid());
    int desc = write(1, buffer, strlen(buffer));
    if (desc < 0){
        perror("Write failed");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < global_argc; i++){ // Transmit SIGUSR1 signal to every child
        if (acc_bool[i] == true){
            int desc = kill(acc_pid[i], SIGUSR1);
            if (desc < 0){
                perror("Kill failed\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

static void info_ChildHandler(){
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "\n[Child Process %d: %d] Value: %d!\n", global_counter, getpid(), z);
    int desc = write(1, buffer, strlen(buffer));
    if (desc < 0){
        perror("Write failed\n");
        exit(EXIT_FAILURE);
    }
}

static void echo_FatherHandler(){
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "\n[Father Process %d] Echo!\n", getpid());
    int desc = write(1, buffer, strlen(buffer));
    if (desc < 0){
        perror("Write failed\n");
        exit(EXIT_FAILURE);
    }
}

static void echo_ChildHandler(){
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "\n[Child Process %d] Echo!\n", getpid());
    int desc = write(1, buffer, strlen(buffer));
    if (desc < 0){
        perror("Write failed\n");
        exit(EXIT_FAILURE);
    }
}

static void alarm_ChildHandler(){
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "[Child Process %d: %d] Time expired! Final value: %d\n", global_counter, getpid(), z);
    int desc = write(1, buffer, strlen(buffer));
    if (desc < 0){
        perror("Write failed\n");
        exit(EXIT_FAILURE);
    }
    desc = kill(getpid(), SIGTERM); // Auto-terminate
    if (desc < 0){
        perror("Kill failed\n");
        exit(EXIT_FAILURE);
    }
}

static void terminate_FatherHandler(){
    char buffer[BUFFER_SIZE];
    write(1, "\n", 1);
    for (int i = 1; i < global_argc+1; i++){
        if (acc_bool[i-1] == true){
            snprintf(buffer, BUFFER_SIZE, "[Father Process %d] Will terminate child process %d: %d\n", getpid(), i, acc_pid[i-1]);
            int desc = write(1, buffer, strlen(buffer));
            if (desc < 0){
                perror("Write failed\n");
                exit(EXIT_FAILURE);
            }
            desc = kill(acc_pid[i-1], SIGTERM);
            if (desc < 0){
                perror("Kill failed\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

static void notify_FatherHandler(){
    pid_t p;
    int status;

    char buffer[BUFFER_SIZE];
    while ((p = waitpid(-1, &status, WNOHANG)) != -1){
        if (p != 0){
            for (int i = 0; i < global_argc; i++){
                if (acc_pid[i] == p){
                    acc_bool[i] = false;
                }
            }
            global_prc--;
            snprintf(buffer, BUFFER_SIZE, "\n[Father Process %d] Number of children running: %d\n", getpid(), global_prc);
            int desc = write(1, buffer, strlen(buffer));
            if (desc < 0){
                perror("Write failed\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

// Starting point
int main(int argc, char **argv){

    pid_t curr;
    int desc, acc_tim[argc-1];
    
    global_argc = argc-1;
    global_prc = global_argc;

    // Setting signal handlers for father-childs
    struct sigaction echoFather, echoChild, infoFather, infoChild, alarmChild, terminateFather, notifyFather;
    
    infoFather.sa_handler = info_FatherHandler;
    echoFather.sa_handler = echo_FatherHandler;
    notifyFather.sa_handler = notify_FatherHandler;
    terminateFather.sa_handler = terminate_FatherHandler;

    infoChild.sa_handler = info_ChildHandler;
    echoChild.sa_handler = echo_ChildHandler;
    alarmChild.sa_handler = alarm_ChildHandler;

    // Parse command-line arguments
    for (int i = 0; i < argc-1; i++){
        acc_tim[i] = atoi(argv[i+1]);
    }
    
    // Printing initial message
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "\nMaximum execution time of children is set to %d secs:\n\n", MAXIMUM_TIME);
    desc = write(1, buffer, strlen(buffer));
    if (desc < 0){
        perror("Write failed\n");
        exit(EXIT_FAILURE);
    }

    // Initializing fork processes
    for (global_counter = 1; global_counter < argc; global_counter++){
        curr = fork();
        if (curr == 0){ // Child process
            // Setting sigaction for child proccess
            sigaction(SIGUSR1, &infoChild, NULL);
            sigaction(SIGUSR2, &echoChild, NULL);
            sigaction(SIGALRM, &alarmChild, NULL);
            snprintf(buffer, BUFFER_SIZE, "[Child Process %d: %d] Was created and will pause!\n", global_counter, getpid());
            desc = write(1, buffer, strlen(buffer));
            if (desc == -1){
                perror("Write failed\n");
                exit(EXIT_FAILURE);
            }
            raise(SIGSTOP); // Auto-stop
            alarm(MAXIMUM_TIME); // Auto-terminate on SIGALRM
            snprintf(buffer, BUFFER_SIZE, "[Child Process %d: %d] Is starting!\n", global_counter, getpid());
            desc = write(1, buffer, strlen(buffer));
            if (desc == -1){
                perror("Write failed\n");
                exit(EXIT_FAILURE);
            }
            z = 0;
            while (1){
                z++;
                sleep(acc_tim[global_counter-1]);
            }
            break; // Exit for loop of father
        }
        else if (curr > 0){ // Father process
            acc_pid[global_counter-1] = curr; // Keeps track of child PIDs
            acc_bool[global_counter-1] = true;
            int wstatus;
            while (waitpid(curr, &wstatus, WUNTRACED) == 0); // Wait until child enters pause state
        }
        else{
            perror("Fork failed\n");
            exit(EXIT_FAILURE);
        }
    }

    if (curr > 0){ // Father transmits SIGCONT to every child
        // Setting sigactions for father
        sigaction(SIGUSR1, &infoFather, NULL);
        sigaction(SIGUSR2, &echoFather, NULL);
        sigaction(SIGCHLD, &notifyFather, NULL);
        sigaction(SIGINT, &terminateFather, NULL);
        sigaction(SIGTERM, &terminateFather, NULL);
        for (int i = 0; i < argc-1; i++){
            int desc = kill(acc_pid[i], SIGCONT);
            if (desc < 0){
                perror("Kill failed\n");
                exit(EXIT_FAILURE);
            }
        }
        snprintf(buffer, BUFFER_SIZE, "\n[Father Process %d] Number of children running: %d\n", getpid(), global_prc);
        desc = write(1, buffer, strlen(buffer));
        if (desc < 0){
            perror("Write failed\n");
            exit(EXIT_FAILURE);
        }
        int status;
        while (wait(&status) > 0);
            // if (WIFSIGNALED(status)){
            //     global_prc--;
            //     snprintf(buffer, BUFFER_SIZE, "\n[Father Process %d] Number of children running: %d\n", getpid(), global_prc);
            //     desc = write(1, buffer, strlen(buffer));
            //     if (desc < 0){
            //         perror("Write failed\n");
            //         exit(EXIT_FAILURE);
            //     }
            // }
        // }
        while (errno == 4){ // In case of EINTR error, re-run wait
            while (wait(&status) > 0); // Wait for every single child to terminate
                // if (WIFSIGNALED(status)){
                //     global_prc--;
                //     snprintf(buffer, BUFFER_SIZE, "\n[Father Process %d] Number of children running: %d\n", getpid(), global_prc);
                //     desc = write(1, buffer, strlen(buffer));
                //     if (desc < 0){
                //         perror("Write failed\n");
                //         exit(EXIT_FAILURE);
                //     }
                // }
            // }
        }
    }
    return 0;
}