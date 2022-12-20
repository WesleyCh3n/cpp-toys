cmake_minimum_required(VERSION 3.10)

project(mqtt-toys)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

if(UNIX)

  # MQTT
  set(MQTTCPP mqttcpp)
  add_library(${MQTTCPP} STATIC IMPORTED)
  set_target_properties(${MQTTCPP} PROPERTIES
    IMPORTED_LOCATION /opt/paho.mqtt.cpp/lib/libpaho-mqttpp3.a
    INTERFACE_INCLUDE_DIRECTORIES /opt/paho.mqtt.cpp/include
  )
  set(MQTTC mqttc)
  add_library(${MQTTC} STATIC IMPORTED)
  set_target_properties(${MQTTC} PROPERTIES
    IMPORTED_LOCATION /opt/paho.mqtt.c/lib/libpaho-mqtt3a.a
    INTERFACE_INCLUDE_DIRECTORIES /opt/paho.mqtt.c/include
  )

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
    target_link_libraries(${file} ${MQTTCPP} ${MQTTC})
  endforeach(srcfile ${APP_SOURCES})

endif(UNIX)
