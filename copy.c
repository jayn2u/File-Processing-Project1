//
// Created by JIWOONG CHOI on 3/13/25.
//
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // 사용법: 프로그램 실행 시 원본파일과 복사본파일의 경로를 인자로 전달합니다.
    if (argc != 3) {
        fprintf(stderr, "사용법: %s <원본파일> <복사본파일>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // 원본파일을 읽기 전용("rb")으로 엽니다.
    FILE *src = fopen(argv[1], "rb");
    if (src == NULL) {
        perror("원본파일 열기 실패");
        return EXIT_FAILURE;
    }

    // 복사본파일을 쓰기("wb") 모드로 엽니다.
    // 이 모드는 파일이 존재할 경우 내용을 모두 삭제(0바이트로 만듦)하며, 없으면 새로 생성합니다.
    FILE *dest = fopen(argv[2], "wb");
    if (dest == NULL) {
        perror("복사본파일 열기 실패");
        fclose(src);
        return EXIT_FAILURE;
    }

    // 10바이트 단위로 데이터를 읽어 복사본파일에 씁니다.
    char buffer[10];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        size_t bytesWritten = fwrite(buffer, 1, bytesRead, dest);
        if (bytesWritten != bytesRead) {
            perror("복사본파일 쓰기 실패");
            fclose(src);
            fclose(dest);
            return EXIT_FAILURE;
        }
    }

    // 열린 파일들을 닫습니다.
    fclose(src);
    fclose(dest);

    return EXIT_SUCCESS;
}