cmake_minimum_required(VERSION 3.18)

project(taurine)
include(cmake/shortcuts.cmake)
enable_testing()

define_library(
  NAME vec
  SOURCES
    vec/vec.c
    vec/vec.h
)

define_library_tests(
  NAME vec_tests
  SOURCE vec/tests.c
  DEPENDENCIES
    vec
)

define_library(
  NAME argparse
  SOURCES
    argparse/argparse.c
    argparse/argparse.h
)

define_library(
  NAME apple_map
  SOURCES
    apple_map/apple_map.c
    apple_map/apple_map.h
)

define_library_tests(
  NAME apple_map_tests
  SOURCE apple_map/tests.c
  DEPENDENCIES
    apple_map
)

define_library(
  NAME arty
  SOURCES
    arty/utf8.c
    arty/utf8.h
    arty/unicode.c
    arty/unicode.h
)

define_library_tests(
  NAME arty_tests
  SOURCE arty/tests.c
  DEPENDENCIES
    arty
)

define_library(
  NAME lunarity
  SOURCES
    lunarity/lexer.h
    lunarity/token.c
    lunarity/lexer.h
    lunarity/lexer.c
    lunarity/location.h
    lunarity/location.c
  DEPENDENCIES
    arty
)

define_library_tests(
  NAME lunarity_lexer_tests
  SOURCE lunarity/lexer_tests.c
  DEPENDENCIES
    lunarity
)

define_cli_exec(
  NAME lunarity
  SOURCE lunarity/cli.c
  DEPENDENCIES
    lunarity
    argparse
)
