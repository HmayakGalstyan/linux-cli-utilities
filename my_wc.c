#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#define BUFF_SIZE 1024

int file_process(int fd, char *filename, size_t *total_lines, size_t *total_words, size_t *total_bytes);

int main(int argc, char *argv[]) {
    size_t total_lines = 0;
    size_t total_words = 0;
    size_t total_bytes = 0;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd == -1) {
            perror("open");
            return 1;
        }

        file_process(fd, argv[i], &total_lines, &total_words, &total_bytes);
        close(fd);
    }

    if (argc > 2) {
        printf("%zu %zu %zu total\n", total_lines, total_words, total_bytes);
    }

    return 0;
}

int file_process(int fd, char *filename, size_t *total_lines, size_t *total_words, size_t *total_bytes) {
    char buffer[BUFF_SIZE];
    ssize_t bytes_read;
                
    size_t lines = 0;
    size_t words = 0;
    size_t bytes = 0;
    int in_word = 0;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0 ) {
        bytes += bytes_read;

        for (ssize_t i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                lines++;
            }

            if (buffer[i] == '\n' || buffer[i] == '\t' || buffer[i] == ' ') {
                in_word = 0;
            } else {
                if (in_word == 0){
                    words++;   
                }
            
                in_word = 1;
            }
            
        }
        
    }

    *total_bytes += bytes;
    *total_words += words;
    *total_lines += lines;

    printf("%zu %zu %zu %s\n", lines, words, bytes, filename);

    return 0;
}
