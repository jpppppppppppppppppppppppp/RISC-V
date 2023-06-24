cmake_minimum_required(VERSION 3.16)
project(SIM)

set(CMAKE_CXX_STANDARD 14)

set(SIM_INCLUDES
        include)
set(SIM_SOURCES
        src/main.cpp
        src/instruction.cpp
        src/LoadStoreBuffer.cpp
        src/memory.cpp
        src/CDB.cpp
        src/RegisterFile.cpp
        src/ReorderBuffer.cpp
        src/ReservationStation.cpp)

add_executable(SIM ${SIM_INCLUDES} ${SIM_SOURCES})
target_include_directories(SIM PRIVATE ${SIM_INCLUDES})