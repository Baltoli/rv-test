cmake_minimum_required(VERSION 3.2)

project(RuntimeVerificationTest)

find_package(LLVM REQUIRED CONFIG)

llvm_map_components_to_libnames(llvm_libs
  passes mcjit executionengine option irreader
  asmparser x86asmparser x86codegen asmprinter
  codegen target transformutils
  bitwriter x86desc bitreader
  core x86info support)

add_executable(rvc
  src/main.cpp)

target_link_libraries(rvc
  ${llvm_libs})

target_include_directories(rvc
  PRIVATE ${LLVM_INCLUDE_DIRS})

target_compile_definitions(rvc
  PRIVATE ${LLVM_DEFINITIONS})

set_property(
  TARGET rvc 
  PROPERTY CXX_STANDARD 14)
