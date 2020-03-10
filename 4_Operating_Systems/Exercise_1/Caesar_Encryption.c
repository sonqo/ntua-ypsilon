#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define BUFFER_SIZE 64

#define VALIDKEY(x) (x >= 0 && x <= 25)
#define VALIDFILE(x) (strcmp((x), "") != 0)
#define STREQUAL(x, y) (strncmp((x), (y), strlen(y)) == 0)

typedef enum { ENCRYPT, DECRYPT } encrypt_mode;

char caesarChar(unsigned char ch, encrypt_mode mode, int key) { // Encrypting characters
    if (ch >= 'a' && ch <= 'z') {
        if (mode == ENCRYPT) {
            ch += key;
            if (ch > 'z') ch -= 26;
        } else {
            ch -= key;
            if (ch < 'a') ch += 26;
        }
        return ch;
    }
    if (ch >= 'A' && ch <= 'Z') {
        if (mode == ENCRYPT) {
            ch += key;
            if (ch > 'Z') ch -= 26;
        } else {
            ch -= key;
            if (ch < 'A') ch += 26;
        }
        return ch;
    }
    return ch;
}

char* caesarString(char* string, encrypt_mode mode, int key, char* result) { // Encrypting strings
    for (int i = 0; i <= strlen(string); i++){
        result[i] = caesarChar(string[i], mode, key);
    }
    return result;
}

void errorRW(){
    printf("Reading or writing failed\n");
    exit(EXIT_FAILURE);
}

void noFile(){
    printf("Input or output files not found\n");
    exit(EXIT_FAILURE);
}

void correctUsage() {
    printf("Valid parameters: [--file [filename] --key [0-25]]\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {

    int fp1, fp2;
    int n_read, n_write;

    char buff[BUFFER_SIZE];
    char result[BUFFER_SIZE];

    int chosen_key = -1;
    const char *chosen_file = NULL;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (STREQUAL(argv[i], "--key")) {
            if ((i == argc - 1) || (i == argc - 3)) {
                correctUsage(); // --key positioned wrong
            }
            else { // argv[i] == "--key", so argv[i+1] has the key choic
                int is_valid_key = VALIDKEY(strtol(argv[i+1], NULL, 10)); // convert string argument to integer
                if (is_valid_key) { // key in range [0-25]
                    chosen_key = strtol(argv[i+1], NULL, 10);
                }
                else {
                    correctUsage();
                }
            }
        }
        else if (STREQUAL(argv[i], "--file")) {
            if ((i == argc - 1) || (i == argc - 3)) {
                correctUsage(); // --file positioned wrong
            }
            else { // argv[i] == "--file", so argv[i+1] has the file name
                int is_valid_file = VALIDFILE(argv[i+1]);
                if (is_valid_file) { // file name not blank
                    chosen_file = argv[i + 1];
                }
                else {
                    correctUsage();
                }
            }
        }
    }

    if (!chosen_file || chosen_key == -1) {
        correctUsage();
    }
    else {
        pid_t first_pid = fork(); // parent initializes C1
        if (first_pid == 0){
            fp1 = open(chosen_file, O_RDONLY);
            fp2 = open("./encrypted.txt", O_WRONLY);
            if (fp1 == -1  || fp2 == -1){ // error opening the specified files
                noFile();
            }
            n_read = read(fp1, buff, BUFFER_SIZE);
            n_write = write(fp2, caesarString(buff, ENCRYPT, chosen_key, result), n_read);
            if (n_read == -1 || n_write < n_read){ // more characters written than read
                errorRW();
            }
            close(fp1);
            close(fp2);
        }
        else {
            wait(NULL); // wait C1 completion
        }
        if (first_pid > 0) {
            pid_t second_pid = fork(); // parent initializes C2
            if (second_pid == 0) {
                fp1 = open("./encrypted.txt", O_RDONLY);
                n_read = read(fp1, buff, BUFFER_SIZE);
                n_write = write(1, caesarString(buff, DECRYPT, chosen_key, result), n_read);
                if (n_read == -1 || n_write < n_read){ // more characters written than read
                    errorRW();
                }
                close(fp1);
            }
            else {
                wait(NULL); // wait C2 completion
            } 
        }
    }
    return 0;
}