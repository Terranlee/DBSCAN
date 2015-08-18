DBSCAN
======

## Introduction  
Grid based DBSCAN algorithm of different versions  

## Algorithms  
1. Brute force (use distance matrix)  
2. KD Tree based algorithm ([KDD96 Paper](https://www.aaai.org/Papers/KDD/1996/KDD96-037.pdf))  
3. Grid based (by [Ade Gunawan](http://alexandria.tue.nl/extra1/afstversl/wsk-i/gunawan2013.pdf))  
4. Reduced precision grid based (approximate DBSCAN with controllable precision)  
5. LSH grid based algorithm (use locality sensitive hashing to merge clusters in any dimension)  

## Compile & run  
>  $cd CPUCode  
>  $cmake CMakeList.txt  
>  $make  
>  $./test_dbscan  

## Data & result  
1. Sample data is in data/  
2. Two results are in result/ 

## Dependencies  
#### Platform and libraries  
1. g++/icpc (c++11 or c++0x supported, see CMakeList.txt for details)  
2. boost  (boost/numeric/ublas)  
3. cmake  

