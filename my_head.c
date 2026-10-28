#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#define BUFF_SIZE 1024

int head_process(int fd, size_t max_lines);

int main(int argc, char *argv[]) {
    int file_start = 1;
    size_t max_lines = 10;

    if (argc >= 3 && strcmp(argv[1], "-n") == 0) {
       max_lines = atoi(argv[2]);
       file_start = 3;
    }

    if (file_start == argc) {
      head_process(STDIN_FILENO, max_lines);
    }

    for (int i = file_start; i < argc; i++) {
      int fd = open(argv[i], O_RDONLY);

      if (fd == -1) {
        perror("open");
        return 1;
      }

      if (argc - file_start > 1) {
        printf("==> %s <==\n", argv[i]);
        fflush(stdout);
      }

      head_process(fd, max_lines);
      close(fd);
    }

    
    return 0;
}

int head_process(int fd, size_t max_lines) {
    char buffer[BUFF_SIZE];
    ssize_t bytes_read;
    size_t lines = 0;
   

    if (max_lines == 0) {
        return 0;
    }

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        ssize_t total_written = 0;
        ssize_t bytes_to_write = bytes_read;
        
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                lines++;
                if (lines == max_lines) {
                  bytes_to_write = i + 1;
                  break;
                }
            }
            
        }

        while (total_written < bytes_to_write) {
          ssize_t write_bytes = write(STDOUT_FILENO, buffer + total_written, bytes_to_write - total_written);

          if (write_bytes == -1) {
            perror("Failed to write in the file");
            return -1;
          }

          if (write_bytes == 0) {
            return -1;
          }

          total_written += write_bytes;
        }

        if (lines == max_lines) {
           return 0;
        }

    }

    if (bytes_read == -1) {
        perror("read");
        return -1;
    }

    return 0;
}