cmake_minimum_required(VERSION 3.1)
project(BytecodeMachine VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

# file(GLOB sources
# "src/*.h"
# "src/*.cpp"
# )

set(sources
    "src/main.cpp"
    "src/Machine.cpp" "include/Machine.h"
)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(BytecodeMachine ${sources})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
