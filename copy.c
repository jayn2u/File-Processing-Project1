//
// Created by JIWOONG CHOI on 3/13/25.
//
#include <stdio.h>
#include <stdlib.h>

int main(const int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "파일의 개수가 잘못되었습니다.\n");
        fprintf(stderr, "사용방법 : ./실행파일 <소스파일> <목적지파일>\n");
        return EXIT_FAILURE;
    }

    FILE *source = fopen(argv[1], "rb");
    if (source == NULL) {
        fprintf(stderr, "소스 파일이 없습니다.\n");
        return EXIT_FAILURE;
    }

    FILE *dest = fopen(argv[2], "wb");
    if (dest == NULL) {
        fprintf(stderr, "목적지 파일이 없습니다.\n");
        return EXIT_FAILURE;
    }

    char buffer[1024];
    size_t bytes_count = 1;
    while (fread(buffer, sizeof(char), bytes_count, source) > 0) {
        fwrite(buffer, sizeof(char), bytes_count, dest);
    }

    fclose(source);
    fclose(dest);

    return EXIT_SUCCESS;
}
