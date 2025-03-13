//
// Created by JIWOONG CHOI on 3/13/25.
//
/**
 * @file read.c
 * @brief 파일의 특정 오프셋부터 지정한 바이트 수만큼 데이터를 읽어 출력하는 프로그램.
 *
 * 사용법:
 *   a.out <오프셋> <바이트 수> <파일명>
 *
 *  - 오프셋: 파일 내에서 데이터를 읽기 시작할 위치 (0부터 시작)
 *  - 바이트 수: 읽을 데이터의 바이트 수 (파일에 남은 데이터가 적으면 존재하는 데이터만 읽음)
 *  - 파일명: 읽을 파일의 이름
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // 인자 개수 확인: 프로그램명 포함 총 4개 인자 필요
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <오프셋> <바이트 수> <파일명>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // 오프셋과 바이트 수를 정수로 변환
    long offset = strtol(argv[1], NULL, 10);
    long byte_count = strtol(argv[2], NULL, 10);
    char *filename = argv[3];

    // 오프셋 또는 바이트 수가 음수인 경우 에러 처리
    if (offset < 0 || byte_count < 0) {
        fprintf(stderr, "Error: 오프셋과 바이트 수는 음수가 될 수 없습니다.\n");
        exit(EXIT_FAILURE);
    }

    // 파일 읽기 전용으로 열기
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Error opening file %s: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // 파일 포인터를 지정된 오프셋으로 이동
    if (lseek(fd, offset, SEEK_SET) == (off_t) -1) {
        fprintf(stderr, "Error seeking in file %s: %s\n", filename, strerror(errno));
        close(fd);
        exit(EXIT_FAILURE);
    }

    // 바이트 수가 0이면 읽을 필요가 없으므로 종료
    if (byte_count == 0) {
        close(fd);
        exit(EXIT_SUCCESS);
    }

    // 읽은 데이터를 저장할 버퍼 동적 할당
    char *buffer = malloc(byte_count);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // 파일로부터 최대 byte_count 바이트 읽기
    ssize_t bytes_read = read(fd, buffer, byte_count);
    if (bytes_read < 0) {
        fprintf(stderr, "Error reading file %s: %s\n", filename, strerror(errno));
        free(buffer);
        close(fd);
        exit(EXIT_FAILURE);
    }

    // 읽은 데이터를 표준 출력(STDOUT)으로 출력
    if (write(STDOUT_FILENO, buffer, bytes_read) < 0) {
        fprintf(stderr, "Error writing to stdout: %s\n", strerror(errno));
        free(buffer);
        close(fd);
        exit(EXIT_FAILURE);
    }

    // 메모리 해제 및 파일 닫기
    free(buffer);
    close(fd);
    return 0;
}