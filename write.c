//
// Created by JIWOONG CHOI on 3/13/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(const int argc, char *argv[]) {
    if (argc != 4) {
        perror("잘못된 인자의 접근입니다.\n");
        perror("사용방법 : a.out <오프셋> <데이터> <파일명>\n");
        return EXIT_FAILURE;
    }

    const long offset = strtol(argv[1], NULL, 10);

    char data[1024];
    if (argv[2][0] == '"' && argv[2][strlen(argv[2]) - 1] == '"') {
        size_t len = strlen(argv[2]);
        if (len < 2) {
            perror("입력된 문자열의 길이가 너무 짧습니다.");
            return EXIT_FAILURE;
        }
        memcpy(data, argv[2] + 1, len - 2);
        data[len - 2] = '\0';
    } else {
        strncpy(data, argv[2], sizeof(data) - 1);
        data[sizeof(data) - 1] = '\0';
    }

    FILE *write = fopen(argv[3], "r+b");
    if (write == NULL) {
        perror("해당 파일이 없습니다.");
        return EXIT_FAILURE;
    }

    if (fseek(write, 0, SEEK_END) != 0) {
        perror("파일 오프셋 이동에 문제가 발생했습니다.");
        fclose(write);
        return EXIT_FAILURE;
    }

    if (fseek(write, offset, SEEK_SET) != 0) {
        perror("파일 오프셋 이동에 문제가 발생했습니다.");
        fclose(write);
        return EXIT_FAILURE;
    }

    if (fwrite(data, sizeof(char), strlen(data), write) != strlen(data)) {
        perror("데이터를 성공적으로 작성하지 못했습니다.");
        return EXIT_FAILURE;
    }

    fclose(write);
    return EXIT_SUCCESS;
}
