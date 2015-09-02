#!/bin/bash
clear

CUHKEXEC=~/CUHK/DBSCAN
EPS=5000
#SIZE=5000000
DIM=5
K=100
ITER=100

NAME=`date '+%H%M'`

for SIZE in `seq 250000 250000 750000`
do

	echo $SIZE
	$CUHKEXEC -algo 0 -n $SIZE -d $DIM -ds input.ds
	./test_dbscan_redu $ITER $EPS $K $DIM $SIZE input.ds >> answer_my3_redu_$NAME
done

for SIZE in `seq 1000000 1000000 10000000`
do
	echo $SIZE
	$CUHKEXEC -algo 0 -n $SIZE -d $DIM -ds input.ds
	./test_dbscan_redu $ITER $EPS $K $DIM $SIZE input.ds >> answer_my3_redu_$NAME
done

DIM=7
for SIZE in `seq 250000 250000 750000`
do
	echo $SIZE
	$CUHKEXEC -algo 0 -n $SIZE -d $DIM -ds input.ds
	./test_dbscan_redu $ITER $EPS $K $DIM $SIZE input.ds >> answer_my5_redu_$NAME
done

for SIZE in `seq 1000000 1000000 10000000`
do
	echo $SIZE
	$CUHKEXEC -algo 0 -n $SIZE -d $DIM -ds input.ds
	./test_dbscan_redu $ITER $EPS $K $DIM $SIZE input.ds >> answer_my5_redu_$NAME
done

DIM=9
for SIZE in `seq 250000 250000 750000`
do
	echo $SIZE
	$CUHKEXEC -algo 0 -n $SIZE -d $DIM -ds input.ds
	./test_dbscan_redu $ITER $EPS $K $DIM $SIZE input.ds >> answer_my7_redu_$NAME
done

for SIZE in `seq 1000000 1000000 10000000`
do
	echo $SIZE
	$CUHKEXEC -algo 0 -n $SIZE -d $DIM -ds input.ds
	./test_dbscan_redu $ITER $EPS $K $DIM $SIZE input.ds >> answer_my7_redu_$NAME
done


