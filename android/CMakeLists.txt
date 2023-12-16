project(react-native-cpp)
cmake_minimum_required(VERSION 3.4.1)

set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 17)

set (PACKAGE_NAME "react-native-cpp")

set(build_DIR ${CMAKE_SOURCE_DIR}/build)

find_package(ReactAndroid REQUIRED CONFIG)
find_library(LOG_LIB log)

add_library(${PACKAGE_NAME}
            SHARED
            cpp-adapter.cpp
            ../cpp/JsiModuleRegistry.cpp
            ${CPP_SOURCES}
)

# Specifies a path to native header files.
include_directories(
            ../cpp
            "${NODE_MODULES_DIR}/react-native/ReactCommon"
)

target_link_libraries(
  ${PACKAGE_NAME}
  ReactAndroid::jsi
  ${LOG_LIB}
)
