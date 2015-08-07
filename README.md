DBSCAN
======

## Introduction  
Grid based DBSCAN algorithm of different versions  
Designed for CPU execute and DataFlow Engine acceleration  

## Algorithms  
1. Brute force (use distance matrix)  
2. KD Tree based algorithm ([KDD96 Paper](https://www.aaai.org/Papers/KDD/1996/KDD96-037.pdf))  
3. Grid based (by [Ade Gunawan](http://alexandria.tue.nl/extra1/afstversl/wsk-i/gunawan2013.pdf))  
4. Reduced precision grid based (approximate DBSCAN with controllable precision)  
5. LSH grid based algorithm (use locality sensitive hashing to merge clusters in any dimension)  
6. Dataflow version1 (acceleration of algorithm 3, supported by Maxeler Dataflow Engine)  
7. Dataflow version2 (acceleration of algorithm 5, supported by Maxeler Dataflow Engine)  

## Compile & run  
#### CPU code  
>  $cd CPUCode  
>  $cmake CMakeList.txt  
>  $make  
>  $./test_dbscan  

#### Dataflow Engine code  
>  $cd EngineCode/hardware/GRID/build  
>  $make runsim  

## Data & result  
1. Sample data is in data/  
2. Two results are in result/ 

## Dependencies  
#### Platform and libraries  
1. g++/icpc (c++11 or c++0x supported, see CMakeList.txt for details)  
2. boost  (boost/numeric/ublas)  
3. cmake  

#### About DataFlow Engine   
Algorithm 6 is in EngineCode/hardware/GRID  
Algorithm 7 is in EngineCode/hardware/LSH  
These algorithms require special platform(Maxeler Dataflow Engine and its special compiler)  

