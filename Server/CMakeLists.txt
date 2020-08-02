cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME "server")
project(${PROJECT_NAME})

find_package( Threads REQUIRED )

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
add_executable(${PROJECT_NAME} ${SOURCES})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME} pthread)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
