cmake_minimum_required(VERSION 3.0)
project(42sh VERSION 1.0.0 LANGUAGES C)

include_directories(src)
add_compile_options(-std=c99)
add_compile_options(-pedantic)
add_compile_options(-Werror)
add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Wvla)
#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)
#add_compile_options(-g)

#Sub CmakeList
add_subdirectory (src/parser)
add_subdirectory (src/others)
add_subdirectory (src/lexer)
add_subdirectory (src/built_in)
add_subdirectory (src/ast)
add_subdirectory (src/exec)
add_subdirectory (src/redirection)
add_subdirectory (src/expansion)


add_executable(42sh




src/built_in/built_in.h
src/built_in/boolean.c
src/built_in/exit.c
src/built_in/echo.c
src/built_in/cd.c
src/built_in/unset.c
src/built_in/dot.c


src/main.c
src/run_program.c
src/run_program.h

)

target_link_libraries(42sh parser others lexer built_in ast exec redirection expansion)
