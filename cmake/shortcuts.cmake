function(define_library)
  set(ONE_VALUE_ARGUMENTS NAME)
  set(MULTI_VALUE_ARGUMENTS SOURCES DEPENDENCIES)
  cmake_parse_arguments(LIBRARY "" "${ONE_VALUE_ARGUMENTS}"
    "${MULTI_VALUE_ARGUMENTS}" ${ARGN})

  add_library(${LIBRARY_NAME} ${LIBRARY_SOURCES})
  target_include_directories(${LIBRARY_NAME} PUBLIC .)
  set_target_properties(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE C)
  target_link_libraries(${LIBRARY_NAME} ${LIBRARY_DEPENDENCIES})
endfunction()

function(define_library_tests)
  set(ONE_VALUE_ARGUMENTS NAME SOURCE)
  set(MULTI_VALUE_ARGUMENTS DEPENDENCIES)
  cmake_parse_arguments(TEST "" "${ONE_VALUE_ARGUMENTS}"
    "${MULTI_VALUE_ARGUMENTS}" ${ARGN})

  add_executable(${TEST_NAME} ${TEST_SOURCE})
  target_link_libraries(${TEST_NAME} ${TEST_DEPENDENCIES})
  add_test(${TEST_NAME} ${TEST_NAME})
endfunction()

function(define_cli_exec)
  set(ONE_VALUE_ARGUMENTS NAME SOURCE)
  set(MULTI_VALUE_ARGUMENTS DEPENDENCIES)
  cmake_parse_arguments(CLI "" "${ONE_VALUE_ARGUMENTS}"
    "${MULTI_VALUE_ARGUMENTS}" ${ARGN})

  add_executable(${CLI_NAME}_cli ${CLI_SOURCE})
  target_link_libraries(${CLI_NAME}_cli ${CLI_DEPENDENCIES})
  set_target_properties(${CLI_NAME}_cli PROPERTIES OUTPUT_NAME ${CLI_NAME})
endfunction()
