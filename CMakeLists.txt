cmake_minimum_required(VERSION 3.20)
project(BytecodeMachine VERSION 0.1.0)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 실행 파일 루트 디렉터리에 생성
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

# 소스파일 선택
file(GLOB sources
"src/*.h"
"src/*.cpp"
)

# 정적으로 빌드
set(CMAKE_EXE_LINKER_FLAGS "-static")

# 헤더파일 경로
include_directories(${CMAKE_SOURCE_DIR}/include)

# 실행파일
add_executable(BytecodeMachine ${sources})
