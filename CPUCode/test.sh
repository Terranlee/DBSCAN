#!/bin/bash
clear

CUHKEXEC=~/CUHK/DBSCAN
EPS=5000
DIM=3
K=100
ITER=100

for SIZE in `seq 1000000 1000000 5000000`
do
	echo $SIZE
	$CUHKEXEC -algo 1 -n $SIZE -d $DIM -r $EPS -k $K -ds data3D/data_$SIZE -rf Clustering_Result >> answer_cuhk
done

DIM=5
for SIZE in `seq 1000000 1000000 4000000`
do
	echo $SIZE
	$CUHKEXEC -algo 1 -n $SIZE -d $DIM -r $EPS -k $K -ds data5D/data_$SIZE -rf Clustering_Result >> answer_cuhk
done

DIM=7
for SIZE in `seq 1000000 1000000 3000000`
do
	echo $SIZE
	$CUHKEXEC -algo 1 -n $SIZE -d $DIM -r $EPS -k $K -ds data7D/data_$SIZE -rf Clustering_Result >> answer_cuhk
done


