cmake_minimum_required(VERSION 3.23)
project(GIS)

set(CMAKE_CXX_STANDARD 14)

add_executable(GIS src/GIS.cpp src/NameIndex.cpp headers/NameIndex.h src/CoordinateIndex.cpp headers/CoordinateIndex.h src/BufferPool.cpp headers/BufferPool.h src/Logger.cpp headers/Logger.h src/CommandProcessor.cpp headers/CommandProcessor.h headers/HashTable.h headers/PRQuadTree.h)

include_directories(headers)

