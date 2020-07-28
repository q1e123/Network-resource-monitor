cmake_minimum_required(VERSION 3.10)
project(Server)

find_package( Threads REQUIRED )

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(Server ${SOURCES})

set_property(TARGET Server PROPERTY CXX_STANDARD 17)
target_link_libraries(Server pthread)
target_compile_options(Server PRIVATE -Wall -Wextra)
