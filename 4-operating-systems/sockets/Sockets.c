#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <netdb.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 256

#define PORTNUM 8080
#define HOST "tcp.akolaitis.os.grnetcloud.net"

#define ACKMSG BLUE"%s"WHITE"\n"
#define CONNECTINGMSG RED"Connecting..."WHITE"\n"
#define DEBUGSENT YELLOW"[DEBUG] Sent: '%s'"WHITE"\n"
#define DEBUGREAD YELLOW"[DEBUG] Read: '%s'"WHITE"\n"
#define VERIFICATION BLUE"Verification code: '%s'"WHITE"\n"
#define CONNECTEDMESG GREEN"Connected to tcp.akolaitis.os.grnetcloud.net"WHITE"\n"
#define RESMSG BLUE"Latest event: %s"WHITE"\n"BLUE"Temperature: %.2f"WHITE"\n"BLUE"Light: %s"WHITE"\n"BLUE"Date: %s"WHITE
#define HELPMSG YELLOW"Available commands:"WHITE"\n"YELLOW"- help: Print help message"WHITE"\n"YELLOW"- exit: Exit"WHITE"\n"YELLOW"- get: Retrieve sensor data"WHITE"\n"YELLOW"- n name surname reason: Ask for permission"WHITE"\n"

#define STREQUAL(x, y) (strncmp((x), (y), strlen(y)) == 0)

#define RED "\033[31;1m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

int main(int argc, char **argv){

    float tf;
    time_t dt;
    int t, desc, sock_fd;
    struct sockaddr_in server;
    struct hostent *server_host;

    char* curr;
    char buffer[BUFFER_SIZE];
    char debugger[BUFFER_SIZE];
    char resultbuffer[BUFFER_SIZE];
    char verificationbuffer[BUFFER_SIZE];
   
    struct Results{
        char* event;
        char* light;
        char* temperature;
        char* datetime;
    };
    struct Results res;

    int domain = AF_INET;
    int type = SOCK_STREAM;

    int flag = 0;
    if (argc == 2){
        if (STREQUAL(argv[1], "--debug")){
            flag = 1;
        }
    }

    sock_fd = socket(domain, type, 0);
    if (sock_fd < 0){
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    if ((server_host=gethostbyname(HOST)) == NULL){
        perror("Unknown host");
        exit(EXIT_FAILURE);
    }

    write(1, CONNECTINGMSG,  strlen(CONNECTINGMSG));
    server.sin_family = AF_INET;
    server.sin_port = htons((u_short) PORTNUM);
    bcopy((char*)server_host->h_addr, (char*)&server.sin_addr, server_host->h_length);

    if (connect(sock_fd, (struct sockaddr*)&server, sizeof(server)) < 0){
        perror("Connection error");
        exit(EXIT_FAILURE);
    }
    write(1, CONNECTEDMESG, strlen(CONNECTEDMESG));

    while(1){
        fd_set inset;
        FD_ZERO(&inset);
        FD_SET(STDIN_FILENO, &inset); // setting select for STD-input
        if (FD_ISSET(STDIN_FILENO, &inset)){ // STD input
            int n_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
            buffer[n_read] = '\0';
            if (n_read > 0 && buffer[n_read-1] == '\n'){ // discard new line character
                buffer[n_read-1] = '\0';
            }
            if (STREQUAL(buffer, "get")){
                write(sock_fd, "get", strlen("get"));
                n_read = read(sock_fd, &buffer, BUFFER_SIZE);
                buffer[n_read] = '\0';
                if (n_read > 0 && buffer[n_read-1] == '\n'){ // discard new line character
                    buffer[n_read-1] = '\0';
                }
                if (flag == 1){
                    snprintf(debugger, BUFFER_SIZE, DEBUGSENT, "get");
                    debugger[strlen(debugger)+1] = '\0';
                    write(1, debugger, strlen(debugger));
                    snprintf(debugger, BUFFER_SIZE, DEBUGREAD, buffer);
                    debugger[strlen(debugger)+1] = '\0';
                    write(1, debugger, strlen(debugger));
                }
                curr = strtok(buffer, " ");
                for (int i=0; i<4; i++){
                    if (i == 0){
                        res.event = curr;
                    }
                    else if (i == 1){
                        res.light = curr;
                    }
                    else if (i == 2){
                        res.temperature = curr;
                    }
                    else if (i == 3){
                        res.datetime = curr;
                    }
                    curr = strtok(NULL, " ");
                }
                t = atoi(res.temperature);
                tf = t/100.0f;
                dt = atoi(res.datetime);
                snprintf(resultbuffer, BUFFER_SIZE, RESMSG, res.event, tf, res.light, ctime(&dt));
                desc = write(1, resultbuffer, strlen(resultbuffer));
                if (desc == -1){
                    perror("Write error");
                    exit(EXIT_FAILURE);
                }
            }
            else if (STREQUAL(buffer, "help")){
                write(1, HELPMSG, strlen(HELPMSG));
            }
            else if (STREQUAL(buffer, "exit")){
                exit(0);
            }
            else{
                write(sock_fd, buffer, strlen(buffer));
                n_read = read(sock_fd, &resultbuffer, BUFFER_SIZE);
                resultbuffer[n_read] = '\0';
                if (n_read > 0 && resultbuffer[n_read-1] == '\n'){ // discard new line character
                    resultbuffer[n_read-1] = '\0';
                }
                if (flag == 1){
                    snprintf(debugger, BUFFER_SIZE, DEBUGSENT, buffer);
                    debugger[strlen(debugger)+1] = '\0';
                    write(1, debugger, strlen(debugger));
                    snprintf(debugger, BUFFER_SIZE, DEBUGREAD, resultbuffer);
                    debugger[strlen(debugger)+1] = '\0';
                    write(1, debugger, strlen(debugger));
                }
                snprintf(verificationbuffer, BUFFER_SIZE, VERIFICATION, resultbuffer);
                write(1, verificationbuffer, strlen(verificationbuffer));
                while(1){
                    fd_set inset;
                    FD_ZERO(&inset);
                    FD_SET(STDIN_FILENO, &inset); // setting select for STD-input
                    if (FD_ISSET(STDIN_FILENO, &inset)){
                        int n_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
                            buffer[n_read] = '\0';
                            if (n_read > 0 && buffer[n_read-1] == '\n'){
                                buffer[n_read-1] = '\0';
                            }
                            write(sock_fd, buffer, strlen(buffer));
                            n_read = read(sock_fd, &resultbuffer, BUFFER_SIZE);
                            resultbuffer[n_read] = '\0';
                            if (n_read > 0 && resultbuffer[n_read-1] == '\n'){ // discard new line character
                                resultbuffer[n_read-1] = '\0';
                            }
                            if (flag == 1){
                                snprintf(debugger, BUFFER_SIZE, DEBUGSENT, buffer);
                                debugger[strlen(debugger)+1] = '\0';
                                write(1, debugger, strlen(debugger));
                                snprintf(debugger, BUFFER_SIZE, DEBUGREAD, resultbuffer);
                                debugger[strlen(debugger)+1] = '\0';
                                write(1, debugger, strlen(debugger));
                            }
                            snprintf(verificationbuffer, BUFFER_SIZE, ACKMSG, resultbuffer);
                            write(1, verificationbuffer, strlen(verificationbuffer));
                            break;
                    }
                }
            }
        }
    }
    exit(0);
}