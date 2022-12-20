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

if (MSVC)
  if(BUILD_STATIC)
    message("build static")
    add_definitions(-DCURL_STATICLIB) # nessasry for static lib curl. see: https://curl.se/docs/faq.html#Link_errors_when_building_libcur
    add_library(CURL STATIC IMPORTED)
    set_target_properties(CURL PROPERTIES # This need to be full path in windows
      IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/3rd/libcurl-vc-x64-release-static-ipv6-sspi-schannel/lib/libcurl_a.lib"
      INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/3rd/libcurl-vc-x64-release-static-ipv6-sspi-schannel/include/"
      )
    set(CURL_LDFLAGS "Ws2_32 Normaliz Crypt32 Wldap32") # only static need extra flags

  else()
    message("build shared")
    add_library(CURL SHARED IMPORTED)
    set_target_properties(CURL PROPERTIES # This need to be full path in windows
      IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/../3rd/libcurl-vc-x64-release-dll-ipv6-sspi-schannel/bin/libcurl.dll"
      IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/../3rd/libcurl-vc-x64-release-dll-ipv6-sspi-schannel/lib/libcurl.lib"
      INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../3rd/libcurl-vc-x64-release-dll-ipv6-sspi-schannel/include/"
      )
    # copy dll to binary folder
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../3rd/libcurl-vc-x64-release-dll-ipv6-sspi-schannel/bin/libcurl.dll Release/libcurl.dll COPYONLY)
  endif(BUILD_STATIC)

  file(GLOB APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)
  foreach(srcfile ${APP_SOURCES})
    file(RELATIVE_PATH filename ${CMAKE_CURRENT_SOURCE_DIR} ${srcfile})
    string(REPLACE ".cc" "" file ${filename})

    add_executable(${file} ${srcfile})
    target_link_libraries(${file} CURL ${CURL_LDFLAGS})
  endforeach(srcfile ${APP_SOURCES})
endif(MSVC)
