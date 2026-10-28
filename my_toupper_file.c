#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#define BUFF_SIZE 1024

int my_toupper_file(int fd);

int main(int argc, char *argv[]) {

    for (int i = 1; i < argc; i++) {
      int fd = open(argv[i], O_RDWR);

      if (fd == -1) {
        perror("open");
        return 1;
      }

      if (argc - 1 > 1) {
        printf("==> %s <==\n", argv[i]);
        fflush(stdout);
      }

      my_toupper_file(fd);
      close(fd);
    }

    
    return 0;
}

int my_toupper_file(int fd) {
    ssize_t read_bytes;
    off_t pos = 0;
    char buffer[BUFF_SIZE];
    while ((read_bytes = pread(fd, buffer, sizeof(buffer), pos)) > 0) {
      ssize_t total_written = 0;
      for (ssize_t i = 0; i < read_bytes; i++) {
        buffer[i] = toupper(buffer[i]);
      }

      while (total_written < read_bytes) {
          ssize_t write_bytes = pwrite(fd, buffer + total_written, read_bytes - total_written, pos + total_written);

          if (write_bytes == -1) {
            perror("Failed to write in the file");
            return -1;
          }

          if (write_bytes == 0) {
            return -1;
          }

          
          total_written += write_bytes;
        }
        pos += read_bytes;
    }
    
    
    return 0;
}