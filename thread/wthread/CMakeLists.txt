cmake_minimum_required(VERSION 3.10)

project(wthrad)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

add_library(wthread STATIC
  ${CMAKE_SOURCE_DIR}/wthread.cc
)

add_executable(wthread_test
  ${CMAKE_SOURCE_DIR}/wthread_test.cc
)

target_link_libraries(wthread_test
  wthread
)
