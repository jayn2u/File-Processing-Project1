//
// Created by JIWOONG CHOI on 3/13/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(const int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "잘못된 접근입니다.\n");
        fprintf(stderr, "사용법 : ./실행파일 <파일1> <파일2> <파일3>\n");
        exit(EXIT_FAILURE);
    }

    FILE *file1 = fopen(argv[1], "wb");
    if (file1 == NULL) {
        fprintf(stderr, "Error opening file1\n");
        exit(EXIT_FAILURE);
    }

    FILE *file2 = fopen(argv[2], "rb");
    if (file2 == NULL) {
        fprintf(stderr, "Error opening file2\n");
        fclose(file1);
        exit(EXIT_FAILURE);
    }

    FILE *file3 = fopen(argv[3], "rb");
    if (file3 == NULL) {
        fprintf(stderr, "Error opening file3\n");
        fclose(file1);
        fclose(file2);
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    const size_t bytes_count = 1;

    while (fread(buffer, sizeof(char), bytes_count, file2) > 0) {
        fwrite(buffer, sizeof(char), bytes_count, file1);
    }

    while (fread(buffer, sizeof(char), bytes_count, file3) > 0) {
        fwrite(buffer, sizeof(char), bytes_count, file1);
    }

    fclose(file1);
    fclose(file2);
    fclose(file3);
    // 내부 검증: 파일 크기 비교
    struct stat stat2, stat3, statOut;
    if (stat(argv[2], &stat2) != 0) {
        fprintf(stderr, "Error getting file2 size\n");
        exit(EXIT_FAILURE);
    }
    if (stat(argv[3], &stat3) != 0) {
        fprintf(stderr, "Error getting file3 size\n");
        exit(EXIT_FAILURE);
    }
    if (stat(argv[1], &statOut) != 0) {
        fprintf(stderr, "Error getting output file size\n");
        exit(EXIT_FAILURE);
    }

    if (statOut.st_size != (stat2.st_size + stat3.st_size)) {
        fprintf(stderr, "병합 파일 크기 불일치: file1 size (%ld) != file2 size (%ld) + file3 size (%ld)\n",
                statOut.st_size, stat2.st_size, stat3.st_size);
        exit(EXIT_FAILURE);
    } else {
        printf("병합 파일 크기 일치 : file1 size (%ld) equals file2 size (%ld) + file3 size (%ld)\n",
               statOut.st_size, stat2.st_size, stat3.st_size);
        exit(EXIT_SUCCESS);
    }
}
