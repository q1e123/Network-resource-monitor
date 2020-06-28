cmake_minimum_required(VERSION 3.10)
project(Siemens)

find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(Siemens ${SOURCES})

set_property(TARGET Siemens PROPERTY CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
target_compile_options(Siemens PRIVATE -Wall -Wextra)
