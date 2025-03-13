//
// Created by JIWOONG CHOI on 3/13/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(const int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "잘못된 접근입니다.\n");
        fprintf(stderr, "접근가이드 : %s <오프셋> <바이트> <파일명>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const long offset = strtol(argv[1], NULL, 10);

    const size_t bytes_to_read = strtoul(argv[2], NULL, 10);
    if (bytes_to_read == 0) {
        return EXIT_SUCCESS;
    }

    const char *filename = argv[3];

    FILE *source = fopen(filename, "rb");
    if (source == NULL) {
        perror("파일 열기 실패");
        return EXIT_FAILURE;
    }

    if (fseek(source, offset, SEEK_SET) != 0) {
        perror("fseek 과정 중 문제가 발생했습니다.");
    }

    char buffer[1024] = {};
    fread(buffer, bytes_to_read, 1, source);
    printf("%s", buffer);

    fclose(source);

    return EXIT_SUCCESS;
}
