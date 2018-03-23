#!/usr/bin/env bash

NN_DIR="NN"

for f in `ls "$NN_DIR"`;do 
	echo "`sed 's/gen_/\nGeneration: /'<<<$f`"
	SUM=0.0
	for NET in `ls "$NN_DIR"/$f/nets`;do
		KW="$(cat "$NN_DIR"/$f/nets/$NET|grep -i 'kw'|sed 's/kw //' )"
		SUM=`echo $SUM + $KW | bc`
		SCORE=`grep "$NET: " "$NN_DIR"/$f/scores 2>/dev/null|sed "s/$NET: //"`
		echo -e "net: $NET  \tkw: $KW\tScore: $SCORE"
	done
	echo Avg KingWeight: `echo "$SUM / 30" | bc -l`
done