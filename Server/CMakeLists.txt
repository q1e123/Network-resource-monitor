cmake_minimum_required(VERSION 3.10)

add_definitions(-DUNICODE -D_UNICODE)
project(server)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/Modules/")

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    find_package(mysql REQUIRED)
    find_package(mysqlcppconn REQUIRED)
endif()

find_package(Threads REQUIRED)

include_directories (${MYSQL_INCLUDE_DIR})

include_directories(include
                    ../Third-party/SimpleIni
                    ../Shared/Logger-lib/include
                    ../Shared/Communication-Protocol
                    ../Shared/System-lib/include
                    ../Shared/Communication-Protocol-lib/include
                    ../Shared/Structs
                    ../Shared/Utils-lib/include
)

file(GLOB SOURCES "src/*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)
add_executable(${PROJECT_NAME} ${SOURCES})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(${PROJECT_NAME} pthread soci_core soci_mysql System_lib Logger_lib Communication_Protocol_lib Utils_lib)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_link_libraries(${PROJECT_NAME} soci_core soci_mysql System_lib Logger_lib Communication_Protocol_lib Utils_lib)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
endif()
