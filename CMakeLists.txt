SET(CMAKE_C_FLAGS "-std=gnu99 -pedantic -fPIC -pedantic -fopenmp -mtune=corei7-avx")
SET(CMAKE_CXX_FLAGS "-std=c++0x -Wall -fPIC -pedantic -fopenmp -mtune=corei7-avx")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)

add_definitions(-DBOOST_UBLAS_NDEBUG)
add_definitions(-std=c++11)

aux_source_directory(. DIR_SRCS)

add_executable(test_dbscan ${DIR_SRCS})

