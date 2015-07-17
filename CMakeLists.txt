SET(CMAKE_C_COMPILER "icc")
SET(CMAKE_CXX_COMPILER "icpc")

SET(CMAKE_C_FLAGS "-std=gnu99 -O3 -pedantic -pedantic")
SET(CMAKE_CXX_FLAGS "-std=c++0x -Wall -O3 -pedantic")
#SET(CMAKE_CXX_FLAGS "-std=c++11 -Wall -O3 -pedantic")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)

add_definitions(-DBOOST_UBLAS_NDEBUG)

aux_source_directory(. DIR_SRCS)

add_executable(test_dbscan ${DIR_SRCS})

