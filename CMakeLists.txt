cmake_minimum_required(VERSION 3.1.0)

project(GDecimal)

# Set the default CMAKE_BUILD_TYPE to Release.
# This should be done before the project command since the latter can set
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_definitions(-std=c++17)
add_definitions(-Wall)
add_definitions(-O2)
add_definitions(-s)

if (MINGW)
  set(LINK_OPTIONS -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -Wl,--whole-archive -lpthread -Wl,--no-whole-archive)
endif ()
if (Clang)
  set(LINK_OPTIONS -static -pthread)
endif ()

# lib
aux_source_directory(Decimal Decimal_SRCS)
add_library(Decimal ${Decimal_SRCS})

# benchmark
aux_source_directory(benchmark benchmark_SRCS)
add_executable(benchmark ${benchmark_SRCS})
target_link_libraries(benchmark ${LINK_OPTIONS})

# test
set(GTEST_ROOT D:/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/googletest)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# set(LINK_OPTIONS -static gtest_main gtest)

aux_source_directory(test test_SRCS)
add_executable(Decimal_test ${test_SRCS})
target_link_libraries(Decimal_test ${LINK_OPTIONS} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})

# 启用测试
enable_testing()

# 测试程序是否成功运行
add_test (test_run bin/Decimal_test)
add_test (benchmark_run bin/benchmark)