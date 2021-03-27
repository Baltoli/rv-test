cmake_minimum_required(VERSION 3.2)

# Run Flex and Bison in the build/ directory
cmake_policy(SET CMP0088 NEW)

project(RuntimeVerificationTest)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
find_package(LLVM REQUIRED CONFIG)

# From https://stackoverflow.com/a/59567896/10121160
# Quick way to get the build system to pick up the Homebrew-installed,
# up-to-date versions of bison and flex.
if (CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin")
    execute_process(
        COMMAND brew --prefix bison 
        RESULT_VARIABLE BREW_BISON
        OUTPUT_VARIABLE BREW_BISON_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (BREW_BISON EQUAL 0 AND EXISTS "${BREW_BISON_PREFIX}")
        message(STATUS "Found Bison keg installed by Homebrew at ${BREW_BISON_PREFIX}")
        set(BISON_EXECUTABLE "${BREW_BISON_PREFIX}/bin/bison")
    endif()

    execute_process(
        COMMAND brew --prefix flex 
        RESULT_VARIABLE BREW_FLEX
        OUTPUT_VARIABLE BREW_FLEX_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (BREW_FLEX EQUAL 0 AND EXISTS "${BREW_FLEX_PREFIX}")
        message(STATUS "Found Flex keg installed by Homebrew at ${BREW_FLEX_PREFIX}")
        set(FLEX_EXECUTABLE "${BREW_FLEX_PREFIX}/bin/flex")
    endif()
endif()

llvm_map_components_to_libnames(llvm_libs bitwriter core support)

flex_target(lexer
  src/lexer.l
  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
  COMPILE_FLAGS -c++)

bison_target(parser
  src/parser.y
  ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)

add_flex_bison_dependency(lexer parser)

add_executable(rvc
  src/main.cpp
  src/ast.cpp
  src/codegen.cpp
  ${FLEX_lexer_OUTPUTS}
  ${BISON_parser_OUTPUT_SOURCE})

target_link_libraries(rvc
  ${FLEX_LIBRARIES}
  ${BISON_LIBRARIES}
  ${llvm_libs})

target_include_directories(rvc
  PRIVATE
    ${BREW_FLEX_PREFIX}/include
    ${LLVM_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR})

target_compile_definitions(rvc
  PRIVATE ${LLVM_DEFINITIONS})

# LLVM requires C++14 internally - would ideally use C++17 but stick with 14 for
# maximum compatibility.
set_property(
  TARGET rvc 
  PROPERTY CXX_STANDARD 14)
