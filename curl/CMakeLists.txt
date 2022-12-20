cmake_minimum_required(VERSION 3.10)

project(curl)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

# if platform is unix based
if(UNIX)
  # use pkg-config to find the curl library
  find_package(PkgConfig)
  pkg_check_modules(CURL REQUIRED libcurl)
  include_directories(${CURL_INCLUDE_DIRS})

  # list the source file in current cmake directory
  file(GLOB APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

  foreach(srcfile ${APP_SOURCES})
    # srcfile: /full/path/to/file_name.cc

    # Preprocess the file variable from full path to file name only
    # you can use message(${variable}) to see the variable output
    file(RELATIVE_PATH filename ${CMAKE_CURRENT_SOURCE_DIR} ${srcfile})
    # filename: file_name.cc
    string(REPLACE ".cc" "" file ${filename})
    # file: file_name

    # add executable
    add_executable(${file} ${srcfile})
    # link executable to the necessary library
    target_link_libraries(${file} ${CURL_STATIC_LDFLAGS})
  endforeach(srcfile ${APP_SOURCES})
  message(${CURL_STATIC_LDFLAGS})

endif(UNIX)
