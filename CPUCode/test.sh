#!/bin/bash 
clear

for i in `seq 10`
do
	echo 10
    ./test_dbscan 10 >> output
done

for i in `seq 10`
do
	echo 20
	./test_dbscan 20 >> output
done

for i in `seq 10`
do
	echo 30
    ./test_dbscan 30 >> output
done

for i in `seq 10`
do
	echo 40
	./test_dbscan 40 >> output
done

for i in `seq 10`
do
	echo 50
	./test_dbscan 50 >> output
done

for i in `seq 10`
do 
	echo 70
	./test_dbscan 70 >> output
done

for i in `seq 10`
do
	echo 100
	./test_dbscan 100 >> output
done

