cmake_minimum_required(VERSION 2.8)

SET(CMAKE_C_COMPILER "icc")
# use icpc & -std=c++0x on server
# use g++ & -std=c++11  on laptop
#SET(CMAKE_CXX_COMPILER "icpc")
SET(CMAKE_CXX_COMPILER "g++")

SET(CMAKE_C_FLAGS "-std=gnu99 -O3 -pedantic -pedantic")
#SET(CMAKE_CXX_FLAGS "-std=c++0x -Wall -O3 -pedantic")
SET(CMAKE_CXX_FLAGS "-std=c++11 -Wall -O3 -pedantic")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED OFF) 
set(Boost_USE_STATIC_RUNTIME OFF)

add_definitions(-DBOOST_UBLAS_NDEBUG)

aux_source_directory(. DIR_SRCS)

LINK_DIRECTORIES(../libs/kdtree2)
INCLUDE_DIRECTORIES(../libs/kdtree2)

add_executable(test_dbscan ${DIR_SRCS})

TARGET_LINK_LIBRARIES(test_dbscan kdtree2)
