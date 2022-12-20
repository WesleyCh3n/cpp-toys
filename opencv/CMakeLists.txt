cmake_minimum_required(VERSION 3.10)

project(opencv)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # this is for the linux user. It can generate compile command for clangd have proper inlcude path
set(CMAKE_CXX_STANDARD 17)

if(MSVC)
  if(BUILD_STATIC)
  else()
    # This example use chocolately opencv
    message("build shared")
    add_library(OPENCV SHARED IMPORTED)
    set_target_properties(OPENCV PROPERTIES # This need to be full path in windows
      IMPORTED_LOCATION "C:/tools/opencv/build/x64/vc15/bin/opencv_world460.dll"
      IMPORTED_IMPLIB "C:/tools/opencv/build/x64/vc15/lib/opencv_world460.lib"
      INTERFACE_INCLUDE_DIRECTORIES "C:/tools/opencv/build/include"
      )
    configure_file("C:/tools/opencv/build/x64/vc15/bin/opencv_world460.dll" Release/opencv_world460.dll COPYONLY)
  endif(BUILD_STATIC)

  # decode encode library
  include_directories("../3rd/cppcodec")

  file(GLOB APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)
  foreach(srcfile ${APP_SOURCES})
    file(RELATIVE_PATH filename ${CMAKE_CURRENT_SOURCE_DIR} ${srcfile})
    string(REPLACE ".cc" "" file ${filename})

    add_executable(${file} ${srcfile})
    target_link_libraries(${file} OPENCV)
  endforeach(srcfile ${APP_SOURCES})
endif(MSVC)
