//
// Created by JIWOONG CHOI on 3/13/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(const int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "잘못된 접근입니다.\n");
        fprintf(stderr, "접근가이드 : %s <오프셋> <바이트> <파일명>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *endptr;
    // 오프셋 파싱 및 유효성 검사
    long offset = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || offset < 0) {
        fprintf(stderr, "오프셋 값이 올바르지 않습니다.\n");
        return EXIT_FAILURE;
    }

    // 읽을 바이트 수 파싱 및 유효성 검사
    unsigned long bytes_to_read = strtoul(argv[2], &endptr, 10);
    if (*endptr != '\0') {
        fprintf(stderr, "바이트 수가 올바르지 않습니다.\n");
        return EXIT_FAILURE;
    }

    // 바이트 수가 0이면 읽을 필요 없음
    if (bytes_to_read == 0) {
        return EXIT_SUCCESS;
    }

    const char *filename = argv[3];
    FILE *source = fopen(filename, "rb");
    if (source == NULL) {
        perror("파일 열기 실패");
        return EXIT_FAILURE;
    }

    // 파일에서 지정된 오프셋으로 이동
    if (fseek(source, offset, SEEK_SET) != 0) {
        perror("fseek 과정 중 문제가 발생했습니다.");
        fclose(source);
        return EXIT_FAILURE;
    }

    // 동적 메모리 할당: 필요한 바이트 수만큼 버퍼 생성
    char *buffer = (char *) malloc(bytes_to_read);
    if (buffer == NULL) {
        perror("메모리 할당 실패");
        fclose(source);
        return EXIT_FAILURE;
    }

    // 파일에서 데이터 읽기
    size_t read_bytes = fread(buffer, 1, bytes_to_read, source);
    if (read_bytes < bytes_to_read && ferror(source)) {
        perror("파일 읽기 중 문제가 발생했습니다.");
        free(buffer);
        fclose(source);
        return EXIT_FAILURE;
    }

    // 읽은 데이터 출력 (실제 읽은 바이트 수만큼 출력)
    if (read_bytes > 0) {
        fwrite(buffer, 1, read_bytes, stdout);
    }

    free(buffer);
    fclose(source);
    return EXIT_SUCCESS;
}
