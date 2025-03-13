//
// Created by JIWOONG CHOI on 3/13/25.
//
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *file1 = fopen(argv[1], "wb");
    if (file1 == NULL) {
        perror("Error opening file1");
        exit(EXIT_FAILURE);
    }

    FILE *file2 = fopen(argv[2], "rb");
    if (file2 == NULL) {
        perror("Error opening file2");
        fclose(file1);
        exit(EXIT_FAILURE);
    }

    FILE *file3 = fopen(argv[3], "rb");
    if (file3 == NULL) {
        perror("Error opening file3");
        fclose(file1);
        fclose(file2);
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    const size_t bytes_count = 1;

    while (fread(buffer, sizeof(char), bytes_count, file2) > 0) {
        fwrite(buffer, sizeof(char), bytes_count, file1);
    }

    while (fread(buffer, sizeof(char), bytes_count, file3) > 0) {
        fwrite(buffer, sizeof(char), bytes_count, file1);
    }

    fclose(file1);
    fclose(file2);
    fclose(file3);
}
