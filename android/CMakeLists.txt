project(react-native-cpp)
cmake_minimum_required(VERSION 3.4.1)

if(NOT DEFINED NODE_MODULES_DIR)
  message(FATAL_ERROR "NODE_MODULES_DIR is not defined")
endif()

message("RNCPP: (CMake) Starting CMake build...")
message("RNCPP: (CMake) Source files: ${CPP_FILES}")
message("RNCPP: (CMake) Includes: ${CPP_INCLUDE_DIRS}")
message("RNCPP: (CMake) node_modules: ${NODE_MODULES_DIR}")

set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 17)

set (PACKAGE_NAME "react-native-cpp")

set(build_DIR ${CMAKE_SOURCE_DIR}/build)

# RN 0.71 distributes prebuilt binaries.
find_package(fbjni REQUIRED CONFIG)
find_package(ReactAndroid REQUIRED CONFIG)

find_library(LOG_LIB log)

add_library(${PACKAGE_NAME}
            SHARED
            cpp-adapter.cpp
            ${CPP_FILES}
            ../cpp/JsiModuleRegistry.cpp
)

include_directories(
            ../cpp
            ${CPP_INCLUDE_DIRS}
            "${NODE_MODULES_DIR}/react-native/ReactCommon"
)

target_link_libraries(
  ${PACKAGE_NAME}
  ReactAndroid::jsi
  fbjni::fbjni
  ${LOG_LIB}
)
