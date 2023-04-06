cmake_minimum_required(VERSION 3.10)

project(thread_project)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

file(GLOB APP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)
foreach(srcfile ${APP_SRC})
  file(RELATIVE_PATH filename ${CMAKE_CURRENT_SOURCE_DIR} ${srcfile})
  string(REPLACE ".cc" "" file ${filename})

  add_executable(${file} ${srcfile})
  target_link_libraries(${file})
endforeach(srcfile ${APP_SRC})

