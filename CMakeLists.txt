cmake_minimum_required(VERSION 3.29)
project(test_AES C)

set(CMAKE_C_STANDARD 11)

add_executable(test_AES main.c
        aes.h
        aes.c
        aes_128.c)
