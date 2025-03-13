//
// Created by JIWOONG CHOI on 3/13/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 메인 함수
 *
 * 사용법: a.out <오프셋> <데이터> <파일명>
 *
 * @param argc 인자의 개수
 * @param argv 인자 배열 (argv[1]: 오프셋, argv[2]: 데이터, argv[3]: 파일명)
 * @return int 프로그램 종료 상태
 */
int main(int argc, char *argv[]) {
    // 1. 인자 개수 검증
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <오프셋> <데이터> <파일명>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // 2. 오프셋 파싱 및 데이터/파일명 저장
    long offset = atol(argv[1]);
    char *data = argv[2];
    char *filename = argv[3];

    // 3. 파일 열기 ("r+b": 읽기/쓰기, 기존 파일 필수)
    FILE *fp = fopen(filename, "r+b");
    if (!fp) {
        perror("파일 열기 실패");
        return EXIT_FAILURE;
    }

    // 4. 파일 포인터를 오프셋 위치로 이동
    if (fseek(fp, offset, SEEK_SET) != 0) {
        perror("파일 위치 이동 실패");
        fclose(fp);
        return EXIT_FAILURE;
    }

    // 5. 데이터 쓰기 (오버라이트)
    size_t data_len = strlen(data);
    size_t written = fwrite(data, 1, data_len, fp);
    if (written != data_len) {
        perror("쓰기 오류");
        fclose(fp);
        return EXIT_FAILURE;
    }

    // 6. 버퍼 플러시 후 파일 닫기
    if (fflush(fp) != 0) {
        perror("fflush 오류");
    }
    fclose(fp);

    return EXIT_SUCCESS;
}
