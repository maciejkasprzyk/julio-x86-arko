cmake_minimum_required(VERSION 3.9)
project(julio)

#verbose
set(CMAKE_VERBOSE_MAKEFILE on)

#asm
enable_language(ASM_NASM)
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> <FLAGS> -f elf64 -o <OBJECT> <SOURCE>")
#set_source_files_properties(x86_function.asm PROPERTIES COMPILE_FLAGS "-g -Fdwarf")
#########################

add_executable(julio
        x86_function.asm
        main.c)

#include allegro
TARGET_LINK_LIBRARIES(julio
        #allegro_image
        #allegro_primitives
        #allegro_font
        allegro
        )

TARGET_LINK_LIBRARIES(julio m)

#all allegro sublibraries listed
#TARGET_LINK_LIBRARIES(julio allegro_acodec
#        allegro_audio
#        allegro_color
#        allegro_dialog
#        allegro_image
#        allegro_main
#        allegro_memfile
#        allegro_physfs
#        allegro_primitives
#        allegro_ttf
#        allegro_font
#        allegro)