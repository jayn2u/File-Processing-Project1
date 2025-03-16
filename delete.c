//
// Created by JIWOONG CHOI on 3/13/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // ftruncate 함수 사용을 위해 필요

#define BUFFER_SIZE 4096

int main(const int argc, const char *argv[]) {
    if (argc != 4) {
        perror("잘못된 인자의 접근");
        perror("사용방법 오류");
        return EXIT_FAILURE;
    }

    char *endptr;
    long offset = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || offset < 0) {
        perror("올바르지 않은 오프셋 값");
        return EXIT_FAILURE;
    }

    size_t bytes_to_delete = strtoul(argv[2], &endptr, 10);
    if (*endptr != '\0') {
        perror("올바르지 않은 바이트 수");
        return EXIT_FAILURE;
    }

    if (bytes_to_delete == 0) {
        return EXIT_SUCCESS;
    }

    FILE *file = fopen(argv[3], "rb+");
    if (file == NULL) {
        perror("파일을 여는 과정에서 문제가 발생했습니다");
        return EXIT_FAILURE;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        perror("파일 끝으로 이동하는데 실패했습니다");
        fclose(file);
        return EXIT_FAILURE;
    }
    long file_size = ftell(file);
    if (file_size == -1L) {
        perror("파일 크기 확인에 실패했습니다");
        fclose(file);
        return EXIT_FAILURE;
    }

    if (offset > file_size) {
        perror("오프셋이 파일 크기를 초과함");
        fclose(file);
        return EXIT_SUCCESS;
    }

    if (offset + bytes_to_delete > file_size) {
        bytes_to_delete = file_size - offset;
    }

    long new_file_size = file_size - bytes_to_delete;
    long src_pos = offset + bytes_to_delete;
    long bytes_to_move = file_size - src_pos;
    long dest_pos = offset;

    char buffer[BUFFER_SIZE];

    while (bytes_to_move > 0) {
        size_t chunk = (bytes_to_move < BUFFER_SIZE) ? bytes_to_move : BUFFER_SIZE;
        if (fseek(file, src_pos, SEEK_SET) != 0) {
            perror("데이터 읽기 위치로 이동 실패");
            fclose(file);
            return EXIT_FAILURE;
        }
        size_t nread = fread(buffer, 1, chunk, file);
        if (nread != chunk) {
            if (ferror(file)) {
                perror("파일 읽기 실패");
                fclose(file);
                return EXIT_FAILURE;
            }
        }
        if (fseek(file, dest_pos, SEEK_SET) != 0) {
            perror("데이터 쓰기 위치로 이동 실패");
            fclose(file);
            return EXIT_FAILURE;
        }
        size_t nwritten = fwrite(buffer, 1, nread, file);
        if (nwritten != nread) {
            perror("파일 쓰기 실패");
            fclose(file);
            return EXIT_FAILURE;
        }
        src_pos += nread;
        dest_pos += nread;
        bytes_to_move -= nread;
    }

    fflush(file);

    // 파일 크기를 줄여 삭제된 영역을 제거
    int fd = fileno(file);
    if (fd == -1) {
        perror("파일 디스크립터 얻기 실패");
        fclose(file);
        return EXIT_FAILURE;
    }
    if (ftruncate(fd, new_file_size) != 0) {
        perror("파일 크기 조정 실패");
        fclose(file);
        return EXIT_FAILURE;
    }

    fclose(file);
    return EXIT_SUCCESS;
}
