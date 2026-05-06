cmake_minimum_required(VERSION 3.16)
project(CppWF VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenSSL 탐색
find_package(OpenSSL REQUIRED)

# 헤더 경로
include_directories(include)

# 라이브러리 소스 (모든 하위 디렉터리 포함)
file(GLOB_RECURSE LIB_SOURCES
    src/http/*.cpp
    src/router/*.cpp
    src/ssl/*.cpp
    src/server/*.cpp
)

add_library(cppwf STATIC ${LIB_SOURCES})
target_include_directories(cppwf PUBLIC include)
target_link_libraries(cppwf OpenSSL::SSL OpenSSL::Crypto)

# 예제 빌드
add_executable(hello_world examples/hello_world.cpp)
target_link_libraries(hello_world cppwf)


# 테스트
enable_testing()
file(GLOB TEST_SOURCES tests/*.cpp)
foreach(TEST_SRC ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SRC})
    target_link_libraries(${TEST_NAME} cppwf)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
