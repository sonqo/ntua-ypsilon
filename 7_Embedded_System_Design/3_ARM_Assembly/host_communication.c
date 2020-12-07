#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 64

int main() {

    int fd, n_read;
    char buffer[BUFFER_SIZE];
    const char port[] = "/dev/ttys001";

    fd = open(port, O_RDWR | O_TRUNC);

    if (fd < 0) {
        printf("Serial port cannot be accessed!\n");
    } else {
        write(1, "Please give a string to send to host: ", BUFFER_SIZE);
        n_read = read(0, buffer, BUFFER_SIZE);
        write(fd, buffer, n_read);
    }
 
    return 0;

}