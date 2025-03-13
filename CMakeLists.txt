cmake_minimum_required(VERSION 3.30)
project(FileProcessingProject1 C)

set(CMAKE_C_STANDARD 11)

add_executable(copy_exec copy.c)
add_executable(merge_exec merge.c)
add_executable(read_exec read.c)
add_executable(write_exec write.c)
add_executable(insert_exec insert.c)
add_executable(delete_exec delete.c)
