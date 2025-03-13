//
// Created by JIWOONG CHOI on 3/13/25.
//
#include <stdio.h>
#include <stdlib.h>

int main(const int argc, char *argv[]) {
    if (argc > 3) {
        printf("파일의 개수를 잘못되었습니다.");
    }

    FILE *source = fopen(argv[1], "rb");
    if (source == NULL) {
        printf("소스 파일이 없습니다.");
        return EXIT_FAILURE;
    }

    FILE *dest = fopen(argv[2], "wb");
    if (dest == NULL) {
        printf("목적지 파일이 없습니다.");
        return EXIT_FAILURE;
    }

    char buffer[1024];

    while (fread(buffer, sizeof(char), 1, source) > 0) {
        fwrite(buffer, sizeof(char), 1, dest);
    }

    fclose(source);
    fclose(dest);

    return EXIT_SUCCESS;
}
