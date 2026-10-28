#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#define BUFF_SIZE 1024

int tail_process(int fd, size_t max_lines);

int main(int argc, char *argv[]) {
    int file_start = 1;
    int max_lines = 10;

    if (argc == file_start) {
      tail_process(STDIN_FILENO, max_lines);
    }

    if (argc >= 3 && strcmp(argv[1], "-n") == 0) {
      max_lines = atoi(argv[2]);
      file_start = 3;
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

      tail_process(fd, max_lines);
      close(fd);
    }
    
    return 0;
}

int tail_process(int fd, size_t max_lines) {
  off_t file_size = lseek(fd, 0, SEEK_END);
  off_t pos = file_size;
  char buff;
  char buffer[BUFF_SIZE];
  ssize_t bytes_read = 0;
  size_t lines = 0;

  while (pos > 0) {
    pos--;
    lseek(fd, pos, SEEK_SET);
    bytes_read = read(fd, &buff, sizeof(buff)); 

    if (pos == file_size - 1 && buff == '\n') {
      continue;
    }
    
    if (buff == '\n') {
      lines++;
      if (lines == max_lines) {
        break;
      }
    }
  }

  lseek(fd, pos, SEEK_SET);

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

