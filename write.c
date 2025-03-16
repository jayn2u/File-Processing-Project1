//
// Created by JIWOONG CHOI on 3/13/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * insert.c
 *
 * 이 프로그램은 이미 존재하는 파일에서 주어진 오프셋와 오프셋+1 사이에 데이터를 끼워 넣습니다.
 * 사용법: a.out <오프셋> <데이터> <파일명>
 *  - 오프셋: 0, 1, 2, ... (파일 내의 위치, 파일의 맨 마지막(EOF 제외)이면 append로 처리됨)
 *  - 데이터: 큰따옴표로 묶여 전달된 문자열 (예: "abc")
 *  - 파일명: 데이터를 삽입할 대상 파일의 이름
 *
 * 실행 후, 결과 파일의 크기는 기존 파일 크기와 삽입할 데이터 길이의 합과 동일해야 합니다.
 */

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "사용법: %s <오프셋> <데이터> <파일명>\n", argv[0]);
        return 1;
    }

    // 명령행 인자에서 오프셋, 데이터, 파일명을 추출
    long offset = atol(argv[1]);
    if (offset < 0) {
        fprintf(stderr, "오프셋은 음수가 될 수 없습니다.\n");
        return 1;
    }

    char *insert_data = argv[2];
    size_t insert_len = strlen(insert_data);
    char *filename = argv[3];

    // 파일을 바이너리 읽기 모드로 오픈
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("파일 열기 실패");
        return 1;
    }

    // 파일 크기를 구함
    if (fseek(fp, 0, SEEK_END) != 0) {
        perror("fseek 실패");
        fclose(fp);
        return 1;
    }
    long file_size = ftell(fp);
    if (file_size < 0) {
        perror("ftell 실패");
        fclose(fp);
        return 1;
    }
    rewind(fp);

    // 파일이 비어있는 경우, 오프셋은 반드시 0이어야 함
    if (file_size == 0) {
        if (offset != 0) {
            fprintf(stderr, "빈 파일에서는 오프셋은 0이어야 합니다.\n");
            fclose(fp);
            return 1;
        }
    } else {
        // 파일이 비어있지 않은 경우, 오프셋은 0 ~ (file_size - 1) 범위여야 함\n
        if (offset > file_size - 1) {
            fprintf(stderr, "오프셋은 파일 크기 범위 내여야 합니다. (파일 크기: %ld)\n", file_size);
            fclose(fp);
            return 1;
        }
    }

    // 기존 파일 내용을 메모리에 읽어옴 (파일이 비어있다면 buffer는 NULL로 유지)
    char *buffer = NULL;
    if (file_size > 0) {
        buffer = (char *)malloc(file_size);
        if (!buffer) {
            perror("메모리 할당 실패");
            fclose(fp);
            return 1;
        }
        size_t read_bytes = fread(buffer, 1, file_size, fp);
        if (read_bytes != file_size) {
            fprintf(stderr, "파일 읽기 오류\n");
            free(buffer);
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);

    // 새 파일 크기: 기존 파일 크기 + 삽입할 데이터 길이
    long new_file_size = file_size + insert_len;
    char *new_buffer = (char *)malloc(new_file_size);
    if (!new_buffer) {
        perror("메모리 할당 실패");
        if (buffer) free(buffer);
        return 1;
    }

    // 삽입 위치 결정\n
    // 파일이 비어있지 않다면, 삽입 위치는 기존 내용의 오프셋까지 복사한 후 그 다음에 데이터를 끼워넣음\n
    // 만약 파일이 빈 경우(first_part_len = 0), 단순히 삽입 데이터를 기록함.
    size_t first_part_len = (file_size > 0) ? (offset + 1) : 0;
    size_t second_part_len = (file_size > 0) ? (file_size - first_part_len) : 0;

    // 기존 파일의 앞부분(0 ~ offset) 복사
    if (first_part_len > 0) {
        memcpy(new_buffer, buffer, first_part_len);
    }

    // 삽입할 데이터 복사
    memcpy(new_buffer + first_part_len, insert_data, insert_len);

    // 기존 파일의 나머지 부분(offset+1 ~ end) 복사
    if (second_part_len > 0) {
        memcpy(new_buffer + first_part_len + insert_len, buffer + first_part_len, second_part_len);
    }

    // 결과를 파일에 기록 (전체 내용을 덮어쓰기)\n
    fp = fopen(filename, "wb");
    if (!fp) {
        perror("파일 열기 실패");
        if (buffer) free(buffer);
        free(new_buffer);
        return 1;
    }
    size_t written = fwrite(new_buffer, 1, new_file_size, fp);
    if (written != new_file_size) {
        fprintf(stderr, "파일 쓰기 오류\n");
        fclose(fp);
        if (buffer) free(buffer);
        free(new_buffer);
        return 1;
    }

    fclose(fp);
    if (buffer) free(buffer);
    free(new_buffer);

    return 0;
}