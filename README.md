DBSCAN
======

## Introduction  
Grid based DBSCAN algorithm of different versions  
Designed for CPU execute and DataFlow Engine acceleration  

## Algorithms  
1. Brute force (use distance matrix)  
2. Grid based (by [Ade Gunawan](http://alexandria.tue.nl/extra1/afstversl/wsk-i/gunawan2013.pdf))  
3. Reduced precision grid based (approximate DBSCAN with controllable precision)  
4. Dataflow version (acceleration supported by Maxeler Dataflow Engine)  

## Compile & run  
#### CPU code  
>  $cd CPUCode  
>  $cmake CMakeList.txt  
>  $make  
>  $./test_dbscan  

#### Dataflow Engine code  
>  $cd EngineCode/hardware/DBSCAN/build  
>  $make runsim  

## Data & result  
1. Sample data is in data/  
2. Two results are in result/ 

## Dependencies  
#### Platform and libraries  
1. g++/icpc (c++11 or c++0x supported)  
2. boost  (boost/numeric/ublas)  
3. cmake  

#### About DataFlow Engine   
The codes related to DataFlow engine are in EngineCode/hardware  
This requires special platform(Maxeler Dataflow Engine and its special compiler)  

