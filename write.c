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

    // 개행 문자 이슈를 해결하기 위해 마지막에 개행이 없으면 추가합니다.
    // 파일의 현재 끝 위치를 확인 후, 마지막 문자가 개행인지 확인합니다.

    if (fseek(write, 0, SEEK_END) == 0) {
        long endPos = ftell(write);
        if (endPos > 0) {
            if (fseek(write, endPos - 1, SEEK_SET) == 0) {
                char lastChar;
                if (fread(&lastChar, 1, 1, write) == 1) {
                    if (lastChar != '\n') {
                        fseek(write, 0, SEEK_END);
                        fputc('\n', write);
                    }
                }
            }
        } else {
            // 파일이 비어있다면 바로 개행을 하나 추가하고 넘어갑니다.
            fputc('\n', write);
        }
    }

    fclose(write);
    return EXIT_SUCCESS;
}
