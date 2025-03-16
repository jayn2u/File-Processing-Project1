//
// Created by JIWOONG CHOI on 3/13/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(const int argc, char *argv[]) {
    if (argc != 4) {
        perror("잘못된 접근입니다.\n");
        perror("사용방법 : a.out <오프셋> <데이터> <파일명>");
        return EXIT_FAILURE;
    }

    const long offset = strtol(argv[1], NULL, 10);

    char data[1024];
    if (argv[2][0] == '"') {
        if (sscanf(argv[2], "\"%[^\"]\"", data) != 1) {
            perror("입력된 문자열에서 큰따옴표 안의 내용을 추출할 수 없습니다.\n");
            return EXIT_FAILURE;
        }
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

    const long file_size = ftell(write);

    long insert_offset = offset;
    if (file_size < insert_offset) {
        insert_offset = file_size;
    }

    if (fseek(write, insert_offset, SEEK_SET) != 0) {
        perror("파일 오프셋 이동에 문제가 발생했습니다.");
        fclose(write);
        return EXIT_FAILURE;
    }

    const size_t remain_file_data_size = file_size - insert_offset;
    char *tail = NULL;
    if (remain_file_data_size > 0) {
        tail = malloc(remain_file_data_size);
        if (tail == NULL) {
            perror("메모리 할당 실패");
            fclose(write);
            return EXIT_FAILURE;
        }
        if (fread(tail, 1, remain_file_data_size, write) != remain_file_data_size) {
            perror("파일 tail 읽기 실패");
            free(tail);
            fclose(write);
            return EXIT_FAILURE;
        }
    }

    if (fseek(write, insert_offset, SEEK_SET) != 0) {
        perror("파일 오프셋 이동에 문제가 발생했습니다.");
        if (tail) free(tail);
        fclose(write);
        return EXIT_FAILURE;
    }

    const size_t data_len = strlen(data);
    if (fwrite(data, 1, data_len, write) != data_len) {
        perror("파일에 데이터를 쓰는 도중 문제가 발생했습니다.");
        if (tail) free(tail);
        fclose(write);
        return EXIT_FAILURE;
    }

    if (remain_file_data_size > 0) {
        if (fwrite(tail, 1, remain_file_data_size, write) != remain_file_data_size) {
            perror("파일 tail 재작성에 실패했습니다.");
            free(tail);
            fclose(write);
            return EXIT_FAILURE;
        }
        free(tail);
    }

    return EXIT_SUCCESS;
}