#!/bin/bash 
clear

for i in `seq 3`
do
    ./test_dbscan 10 >> result
done

for i in `seq 5`
do
    ./test_dbscan 20 >> result
done
