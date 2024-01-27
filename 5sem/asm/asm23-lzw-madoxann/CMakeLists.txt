cmake_minimum_required(VERSION 3.26)
project(hw2 C)

include_directories(.)
enable_language(ASM_NASM)
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> \
    <FLAGS> -f ${CMAKE_ASM_NASM_OBJECT_FORMAT} -o <OBJECT> <SOURCE>")

set_source_files_properties(lzw.asm PROPERTIES COMPILE_FLAGS "-g -Fdwarf")
set_source_files_properties(tmain.c PROPERTIES COMPILE_FLAGS "-m32 -no-pie -fno-pie")

add_link_options(-m32 -no-pie -fno-pie)

set(CMAKE_C_STANDARD 11)

add_executable(hw2 lzw.asm tmain.c)
