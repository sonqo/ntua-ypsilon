#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define VALIDKEY(x) (x >= 0 && x <= 25)
#define VALIDFILE(x) (strcmp((x), "") != 0)
#define STREQUAL(x, y) (strncmp((x), (y), strlen(y)) == 0)

#define BUFFER_SIZE 64
#define MESSAGE "This is my secret message!\n"

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
    for (int i=0; i<=strlen(string); i++){
        result[i] = caesarChar(string[i], mode, key);
    }
    return result;
}

void correctUsage(const char *prog) {
    printf("Usage: %s [--file [filename] --key [0-25]]\n", prog);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {

    FILE *fp;

    char buff[BUFFER_SIZE];
    char result[(int)(strlen(MESSAGE))];

    int chosen_key = -1;
    const char *chosen_file = NULL;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (STREQUAL(argv[i], "--key")) {
            if ((i == argc - 1) || (i == argc - 3)) {
                correctUsage(argv[0]); // --key positioned wrong
            }
            else { // argv[i] == "--key", so argv[i+1] has the key choice

                int is_valid_key = VALIDKEY(strtol(argv[i+1], NULL, 10)); // convert string argument to integer

                if (is_valid_key) { // key in range [0-25]
                    chosen_key = strtol(argv[i+1], NULL, 10);
                }
                else {
                    correctUsage(argv[0]);
                }
            }
        }
        else if (STREQUAL(argv[i], "--file")) {
            if ((i == argc - 1) || (i == argc - 3)) {
                correctUsage(argv[0]); // --file positioned wrong
            }
            else { // argv[i] == "--file", so argv[i+1] has the file name

                int is_valid_file = VALIDFILE(argv[i+1]);

                if (is_valid_file) { // file name not blank
                    chosen_file = argv[i + 1];
                }
                else {
                    correctUsage(argv[0]);
                }
            }
        }
    }

    if (!chosen_file || chosen_key == -1) {
        correctUsage(argv[0]);
    }
    // Initialize processes
    else {
        pid_t first_pid = fork(); // parent initializes C1
        if (first_pid == 0){
            fp = fopen(chosen_file, "w");
            fprintf(fp, "%s", caesarString(MESSAGE, ENCRYPT, chosen_key, result));
            fclose(fp);
            // printf("CHILD_1: My pid is: %d, my father is: %d\n", getpid(), getppid());
        }
        else {
            // printf("PARENT: My pid is: %d\n", getpid());
            wait(NULL); // wait C1
        }
        if (first_pid > 0) {
            pid_t second_pid = fork(); // parent initializes C2
            if (second_pid == 0) {
                fp = fopen(chosen_file, "r");
                fgets(buff, BUFFER_SIZE, fp);
                fclose(fp);
                // printf("CHILD_2: My pid is: %d, my father is: %d\n", getpid(), getppid());
                printf("%s", caesarString(buff, DECRYPT, chosen_key, result));
            }
            else {
                // printf("PARENT: My pid is: %d\n", getpid());
                wait(NULL);
            } 
        }
    }
    return 0;
}