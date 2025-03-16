//
// Created by JIWOONG CHOI on 3/13/25.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(const int argc, char *argv[]) {
    if (argc != 4) {
        perror("잘못된 인자의 접근입니다.\n");
        perror("사용방법 : a.out <오프셋> <데이터> <파일명>\n");
        return EXIT_FAILURE;
    }
}