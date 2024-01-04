cmake_minimum_required(VERSION 3.18)

project(taurine)

add_library(apple_map
        apple_map/src/apple_map.c
        apple_map/include/apple_map.h)

target_include_directories(apple_map PUBLIC .)
set_target_properties(apple_map PROPERTIES LINKER_LANGUAGE C)

enable_testing()
add_executable(apple_map_tests apple_map/tests/tests.c)
target_link_libraries(apple_map_tests apple_map)
add_test(apple_map_tests apple_map_tests)

add_library(arty
        arty/src/utf8.c
        arty/include/utf8.h
        arty/src/unicode.c
        arty/include/unicode.h)
target_include_directories(arty PUBLIC .)

set_target_properties(arty PROPERTIES LINKER_LANGUAGE C)

enable_testing()
add_executable(arty_tests arty/tests/tests.c)
target_link_libraries(arty_tests arty)
add_test(arty_tests arty_tests)

add_library(lunarity
        lunarity/include/lexer.h
        lunarity/src/token.c
        lunarity/include/lexer.h
        lunarity/src/lexer.c
        lunarity/include/location.h
        lunarity/src/location.c)
target_include_directories(lunarity PUBLIC .)

enable_testing()
add_executable(lunarity_lexer_tests lunarity/tests/lexer_tests.c)
target_link_libraries(lunarity_lexer_tests lunarity arty)
target_include_directories(lunarity_lexer_tests PUBLIC ..)
add_test(lunarity_lexer_tests lunarity_lexer_tests)
