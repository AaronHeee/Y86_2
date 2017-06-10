cmake_minimum_required(VERSION 3.6)
project(kernel)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Stage.h Stage.cpp ALU.cpp ALU.h MemFile.cpp MemFile.h Assembler.cpp Assembler.h RegFile.cpp RegFile.h)
add_executable(kernel ${SOURCE_FILES} Stage.h Stage.cpp ALU.cpp ALU.h)

find_package(Threads REQUIRED)
target_link_libraries(kernel Threads::Threads)