cmake_minimum_required(VERSION 3.18)

project(taurine)
include(cmake/shortcuts.cmake)
enable_testing()

define_library(
  NAME vec
  SOURCES
    vec/src/vec.h
  MAP_HEADERS_TO_ROOT_DIR
)

define_library(
  NAME tau
  SOURCES
    tau/src/tau.h
    tau/src/misc.h
    tau/src/types.h
    tau/src/compilers.h
  MAP_HEADERS_TO_ROOT_DIR
)

define_library_tests(
  NAME vec_with_linear_growth_tests
  SOURCE vec/tests/tests.c
  DEPENDENCIES
    vec
    tau
)

define_library_tests(
  NAME vec_with_logarithmic_growth_tests
  SOURCE vec/tests/tests.c
  DEPENDENCIES
    vec
    tau
)

target_compile_definitions(vec_with_linear_growth_tests PRIVATE VEC_LINEAR_GROWTH)
target_compile_definitions(vec_with_logarithmic_growth_tests PRIVATE VEC_LOGARITHMIC_GROWTH)

define_library(
  NAME argparse
  SOURCES
    argparse/src/argparse.c
    argparse/src/argparse.h
  MAP_HEADERS_TO_ROOT_DIR
)

define_library(
  NAME apple_map
  SOURCES
    apple_map/src/apple_map.c
    apple_map/src/apple_map.h
  MAP_HEADERS_TO_ROOT_DIR
)

define_library_tests(
  NAME apple_map_tests
  SOURCE apple_map/tests/tests.c
  DEPENDENCIES
    apple_map
)

define_library(
  NAME arty
  SOURCES
    arty/src/utf8.c
    arty/src/utf8.h
    arty/src/unicode.c
    arty/src/unicode.h
  MAP_HEADERS_TO_ROOT_DIR
)

define_library_tests(
  NAME arty_tests
  SOURCE arty/tests/tests.c
  DEPENDENCIES
    arty
)

define_library(
  NAME lunarity
  SOURCES
    lunarity/src/token.h
    lunarity/src/token.c
    lunarity/src/lexer.h
    lunarity/src/lexer.c
    lunarity/src/location.h
    lunarity/src/location.c
  DEPENDENCIES
    arty
  MAP_HEADERS_TO_ROOT_DIR
)

define_library_tests(
  NAME lunarity_lexer_tests
  SOURCE lunarity/tests/lexer_tests.c
  DEPENDENCIES
    lunarity
)

define_cli_exec(
  NAME lunarity
  SOURCE lunarity/src/cli.c
  DEPENDENCIES
    lunarity
    argparse
)
