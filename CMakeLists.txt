cmake_minimum_required(VERSION 3.0.0)
project(BytecodeMachine VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB sources
     "src/*.h"
     "src/*.cpp"
)

add_executable(BytecodeMachine ${sources})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
