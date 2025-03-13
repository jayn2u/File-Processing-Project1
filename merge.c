//
// Created by JIWOONG CHOI on 3/13/25.
//
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <파일명1> <파일명2> <파일명3>\n", argv[0]);
        return 1;
    }

    FILE *fp_out = fopen(argv[1], "wb");
    if (!fp_out) {
        perror("Error opening output file");
        return 1;
    }

    FILE *fp_in1 = fopen(argv[2], "rb");
    if (!fp_in1) {
        perror("Error opening input file");
        fclose(fp_out);
        return 1;
    }

    FILE *fp_in2 = fopen(argv[3], "rb");
    if (!fp_in2) {
        perror("Error opening input file");
        fclose(fp_out);
        fclose(fp_in1);
        return 1;
    }

    char buffer[1024];
    size_t bytesRead;

    // Merge contents from the second file
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), fp_in1)) > 0) {
        if (fwrite(buffer, 1, bytesRead, fp_out) != bytesRead) {
            perror("Error writing to output file");
            fclose(fp_out);
            fclose(fp_in1);
            fclose(fp_in2);
            return 1;
        }
    }

    // Merge contents from the third file
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), fp_in2)) > 0) {
        if (fwrite(buffer, 1, bytesRead, fp_out) != bytesRead) {
            perror("Error writing to output file");
            fclose(fp_out);
            fclose(fp_in1);
            fclose(fp_in2);
            return 1;
        }
    }

    fclose(fp_out);
    fclose(fp_in1);
    fclose(fp_in2);

    return 0;
}