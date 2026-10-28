#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#define BUFF_SIZE 1024

int catting(int fd);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        catting(STDIN_FILENO);
        return 0;
    }

    for(int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd == -1) {
            perror("failed open file");
            return -1;
        }

        catting(fd);
        close(fd);
        
    }

    return 0;
}

int catting(int fd) {
    char buffer[BUFF_SIZE];
    ssize_t bytes_read;
    

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0 ) {
        ssize_t total_written = 0;
        while (total_written < bytes_read) {
          ssize_t write_bytes = write(STDOUT_FILENO, buffer + total_written, bytes_read - total_written);

          if (write_bytes == -1) {
            perror("Failed to write in the file");
            return -1;
          }

          if (write_bytes == 0) {
            return -1;
          }

          total_written += write_bytes;
        }

        
    }
    
    if (bytes_read == -1) {
        perror("Failed to read");
        return -1;
    }

    return 0;   
}