cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME "client")
project(${PROJECT_NAME})

find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

include_directories(include
                    ../Shared/System-lib/include
                    ../Shared/Utils-lib/include
                    ../Shared/Logger-lib/include
                    ../Shared/Communication-Protocol-lib/include
                    ../Third-party/SimpleIni
                    ../Third-party/Mathplot/include
)
file(GLOB SOURCES "src/*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

add_executable(${PROJECT_NAME} ${SOURCES})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME} 
                        ${wxWidgets_LIBRARIES} 
                        System_lib Utils_lib Logger_lib 
                        Communication_Protocol_lib
                        Mathplot_lib)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
endif()

