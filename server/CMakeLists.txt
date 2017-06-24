cmake_minimum_required(VERSION 3.6)
project(server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES server.cpp ALU.cpp ALU.h MemFile.cpp MemFile.h Assembler.cpp Assembler.h RegFile.cpp RegFile.h Stage.cpp Stage.h)
add_executable(server ${SOURCE_FILES} Assembler.cpp Assembler.h RegFile.cpp RegFile.h Stage.cpp Stage.h)

find_package(Threads REQUIRED)
target_link_libraries(server Threads::Threads)