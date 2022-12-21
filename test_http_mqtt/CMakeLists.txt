cmake_minimum_required(VERSION 3.10)

project(curl)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

if (MSVC)
  if(BUILD_STATIC)
    message("build static")
    # CURL
    add_definitions(-DCURL_STATICLIB) # nessasry for static lib curl. see: https://curl.se/docs/faq.html#Link_errors_when_building_libcur
    add_library(CURL STATIC IMPORTED)
    set_target_properties(CURL PROPERTIES # This need to be full path in windows
      IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/../3rd/libcurl-vc-x64-release-static-ipv6-sspi-schannel/lib/libcurl_a.lib"
      INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../3rd/libcurl-vc-x64-release-static-ipv6-sspi-schannel/include/"
      )
    set(CURL_LDFLAGS Ws2_32 Normaliz Crypt32 Wldap32) # only static need extra flags

    # MQTT C
    add_library(MQTTC3A STATIC IMPORTED)
    set_target_properties(MQTTC3A PROPERTIES # This need to be full path in windows
      IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/../3rd/paho.mqtt.c.static/lib/paho-mqtt3a-static.lib"
      INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../3rd/paho.mqtt.c.static/include/"
      )
    # MQTT CPP
    add_library(MQTTCPP STATIC IMPORTED)
    set_target_properties(MQTTCPP PROPERTIES # This need to be full path in windows
      IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/../3rd/paho.mqtt.cpp.static/lib/paho-mqttpp3-static.lib"
      INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../3rd/paho.mqtt.cpp.static/include/"
      )
    set(MQTT_LDFALGS Ws2_32)
  else()
    message("build shared")
  endif(BUILD_STATIC)

  add_library(OPENCV SHARED IMPORTED)
  set_target_properties(OPENCV PROPERTIES # This need to be full path in windows
    IMPORTED_LOCATION "C:/tools/opencv/build/x64/vc15/bin/opencv_world460.dll"
    IMPORTED_IMPLIB "C:/tools/opencv/build/x64/vc15/lib/opencv_world460.lib"
    INTERFACE_INCLUDE_DIRECTORIES "C:/tools/opencv/build/include"
    )
  configure_file("C:/tools/opencv/build/x64/vc15/bin/opencv_world460.dll" Release/opencv_world460.dll COPYONLY)

  add_executable(curl_send ./curl_send.cc)
  target_link_libraries(curl_send CURL ${CURL_LDFLAGS})

  include_directories("../3rd/cppcodec")

  add_executable(mqtt_pub ./mqtt_pub.cc)
  target_compile_options(mqtt_pub PRIVATE /wd4834) # disable warning C4834
  target_link_libraries(mqtt_pub MQTTCPP MQTTC3A ${MQTT_LDFALGS} OPENCV)

  add_executable(mqtt_sub ./mqtt_sub.cc)
  target_compile_options(mqtt_sub PRIVATE /wd4834) # disable warning C4834
  target_link_libraries(mqtt_sub MQTTCPP MQTTC3A ${MQTT_LDFALGS} OPENCV)

endif(MSVC)
