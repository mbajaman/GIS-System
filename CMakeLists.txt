cmake_minimum_required(VERSION 3.23)
project(GIS)

set(CMAKE_CXX_STANDARD 14)

add_executable(GIS src/COMP8042A2Test.cpp src/GIS.cpp src/GISRecord.cpp headers/GISRecord.h src/NameIndex.cpp headers/NameIndex.h src/CoordinateIndex.cpp headers/CoordinateIndex.h src/BufferPool.cpp headers/BufferPool.h src/Logger.cpp headers/Logger.h src/SystemManager.cpp headers/SystemManager.h src/CommandProcessor.cpp headers/CommandProcessor.h)

include_directories(headers)

