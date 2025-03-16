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
    if (argv[2][0] == '"' && argv[2][strlen(argv[2]) - 1] == '"') {
        size_t len = strlen(argv[2]);
        if (len < 2) {
            perror("입력된 문자열의 길이가 너무 짧습니다.");
            return EXIT_FAILURE;
        }
        memcpy(data, argv[2] + 1, len - 2);
        data[len - 2] = '\0';
    } else {
        strncpy(data, argv[2], sizeof(data) - 1);
        data[sizeof(data) - 1] = '\0';
    }

    FILE *insert = fopen(argv[3], "r+b");
    if (insert == NULL) {
        perror("해당 파일이 없습니다.");
        return EXIT_FAILURE;
    }

    if (fseek(insert, 0, SEEK_END) != 0) {
        perror("파일 오프셋 이동에 문제가 발생했습니다.");
        fclose(insert);
        return EXIT_FAILURE;
    }

    const long file_size = ftell(insert);

    long insert_offset = offset;
    if (file_size < insert_offset) {
        insert_offset = file_size;
    }

    if (fseek(insert, insert_offset, SEEK_SET) != 0) {
        perror("파일 오프셋 이동에 문제가 발생했습니다.");
        fclose(insert);
        return EXIT_FAILURE;
    }

    const size_t remain_file_data_size = file_size - insert_offset;
    char *tail = NULL;
    if (remain_file_data_size > 0) {
        tail = malloc(remain_file_data_size);
        if (tail == NULL) {
            perror("메모리 할당 실패");
            fclose(insert);
            return EXIT_FAILURE;
        }
        if (fread(tail, 1, remain_file_data_size, insert) != remain_file_data_size) {
            perror("파일 tail 읽기 실패");
            free(tail);
            fclose(insert);
            return EXIT_FAILURE;
        }
    }

    if (fseek(insert, insert_offset, SEEK_SET) != 0) {
        perror("파일 오프셋 이동에 문제가 발생했습니다.");
        if (tail) free(tail);
        fclose(insert);
        return EXIT_FAILURE;
    }

    const size_t data_len = strlen(data);
    if (fwrite(data, 1, data_len, insert) != data_len) {
        perror("파일에 데이터를 쓰는 도중 문제가 발생했습니다.");
        if (tail) free(tail);
        fclose(insert);
        return EXIT_FAILURE;
    }

    if (remain_file_data_size > 0) {
        if (fwrite(tail, 1, remain_file_data_size, insert) != remain_file_data_size) {
            perror("파일 tail 재작성에 실패했습니다.");
            free(tail);
            fclose(insert);
            return EXIT_FAILURE;
        }
        free(tail);
    }

    return EXIT_SUCCESS;
}
