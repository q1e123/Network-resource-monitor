cmake_minimum_required(VERSION 3.10)

project("Logger_lib")

ADD_DEFINITIONS(-DLOGGER_SHARED)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_modules/;${CMAKE_MODULE_PATH}")

include_directories(include
                    ../../Third-party/SimpleIni
)

file(GLOB SRC_FILES "src/*.cpp")
FILE(GLOB HEADER_FILES "include/*.h")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
SET(CMAKE_CXX_STANDARD 17)

ADD_LIBRARY(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SRC_FILES})

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
endif()


