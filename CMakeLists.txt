cmake_minimum_required(VERSION 3.18)

project(taurine)
include(cmake/shortcuts.cmake)
enable_testing()

define_library(
  NAME argparse
  SOURCES
    argparse/src/argparse.c
    argparse/include/argparse.h
)

define_library(
  NAME apple_map
  SOURCES
    apple_map/src/apple_map.c
    apple_map/include/apple_map.h
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
    arty/include/utf8.h
    arty/src/unicode.c
    arty/include/unicode.h
)

define_library(
  NAME lunarity
  SOURCES
    lunarity/include/lexer.h
    lunarity/src/token.c
    lunarity/include/lexer.h
    lunarity/src/lexer.c
    lunarity/include/location.h
    lunarity/src/location.c
  DEPENDENCIES
    arty
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
