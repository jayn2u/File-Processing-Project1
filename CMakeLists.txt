cmake_minimum_required(VERSION 3.30)
project(FileProcessingProject1 C)

set(CMAKE_C_STANDARD 11)

add_executable(FileProcessingProject1
        copy.c
        merge.c
        read.c
        write.c
        insert.c
        delete.c)
