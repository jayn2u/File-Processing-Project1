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

    FILE *dest = fopen(argv[2], "wb"); // 존재하면 기존 내용 삭제, 없으면 새로 생성
    if (dest == NULL) {
        fprintf(stderr, "목적지 파일을 열 수 없습니다.\n");
        fclose(source);
        return EXIT_FAILURE;
    }

    char buffer[10];
    size_t bytes_read;
    // 10바이트 단위로 읽어, 마지막 남은 데이터도 처리
    while ((bytes_read = fread(buffer, 1, 10, source)) > 0) {
        fwrite(buffer, 1, bytes_read, dest);
    }

    fclose(source);
    fclose(dest);

    return EXIT_SUCCESS;
}
