set(MAPPED_HEADERS_DIR ${CMAKE_BINARY_DIR}/.mapped_headers)

macro(define_library)
  set(OPTIONAL_ARGUMENTS MAP_HEADERS_TO_ROOT_DIR)
  set(ONE_VALUE_ARGUMENTS NAME)
  set(MULTI_VALUE_ARGUMENTS SOURCES DEPENDENCIES)
  cmake_parse_arguments(LIBRARY "${OPTIONAL_ARGUMENTS}" "${ONE_VALUE_ARGUMENTS}"
    "${MULTI_VALUE_ARGUMENTS}" ${ARGN})

  add_library(${LIBRARY_NAME} ${LIBRARY_SOURCES})
  target_include_directories(${LIBRARY_NAME} PRIVATE .)
  target_include_directories(${LIBRARY_NAME} PRIVATE ${MAPPED_HEADERS_DIR})

  set_target_properties(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE C)
  target_link_libraries(${LIBRARY_NAME} ${LIBRARY_DEPENDENCIES})

  if(${LIBRARY_MAP_HEADERS_TO_ROOT_DIR})
    file(MAKE_DIRECTORY ${MAPPED_HEADERS_DIR}/${LIBRARY_NAME})
    foreach(source ${LIBRARY_SOURCES})
      get_filename_component(ext ${source} LAST_EXT)
      if(${ext} STREQUAL ".h")
        get_filename_component(header_filename_without_dir ${source} NAME)
        get_filename_component(source ${source} ABSOLUTE)

        configure_file(${source} ${MAPPED_HEADERS_DIR}/${LIBRARY_NAME}/${header_filename_without_dir} COPYONLY)

        message("-- Mapping ${source} to ${MAPPED_HEADERS_DIR}/${LIBRARY_NAME}/${header_filename_without_dir} done")
      endif()
    endforeach()
  endif()
endmacro()

macro(define_library_tests)
  set(ONE_VALUE_ARGUMENTS NAME SOURCE)
  set(MULTI_VALUE_ARGUMENTS DEPENDENCIES)
  cmake_parse_arguments(TEST "" "${ONE_VALUE_ARGUMENTS}"
    "${MULTI_VALUE_ARGUMENTS}" ${ARGN})

  add_executable(${TEST_NAME} ${TEST_SOURCE})
  target_link_libraries(${TEST_NAME} ${TEST_DEPENDENCIES})

  target_include_directories(${TEST_NAME} PRIVATE .)
  target_include_directories(${TEST_NAME} PRIVATE ${MAPPED_HEADERS_DIR})

  add_test(${TEST_NAME} ${TEST_NAME})
endmacro()

macro(define_cli_exec)
  set(ONE_VALUE_ARGUMENTS NAME SOURCE)
  set(MULTI_VALUE_ARGUMENTS DEPENDENCIES)
  cmake_parse_arguments(CLI "" "${ONE_VALUE_ARGUMENTS}"
    "${MULTI_VALUE_ARGUMENTS}" ${ARGN})

  add_executable(${CLI_NAME}_cli ${CLI_SOURCE})
  target_link_libraries(${CLI_NAME}_cli ${CLI_DEPENDENCIES})
  target_include_directories(${CLI_NAME}_cli PRIVATE .)
  target_include_directories(${CLI_NAME}_cli PRIVATE ${MAPPED_HEADERS_DIR})
  set_target_properties(${CLI_NAME}_cli PROPERTIES OUTPUT_NAME ${CLI_NAME})
endmacro()
