cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0043 OLD)
project(polybobin)

option(BUILD_CONAN "Build with conan" OFF)

if(BUILD_CONAN)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
endif()

option(BUILD_TESTS "Build tests" ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# Include custom CMake modules (for finding GLM, GMock)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if(NOT BUILD_CONAN)
    find_package(wxWidgets 3.1.0 COMPONENTS core base gl REQUIRED)
endif()

include("${wxWidgets_USE_FILE}")
set(wxWidgets_USE_STATIC ON)

set(polybobin_lib polybolib)

if(WIN32)
    # Hides warnings about deprecated functions
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_subdirectory(src)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
