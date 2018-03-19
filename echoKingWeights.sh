#!/usr/bin/env bash

for f in `ls NN`;do 
	echo "`sed 's/gen_/\nGeneration: /'<<<$f`"
	SUM=0.0
	for NET in `ls NN/$f/nets`;do
		KW="$(cat NN/$f/nets/$NET|grep -i 'kw'|sed 's/kw //' )"
		SUM=`echo $SUM + $KW | bc`
		SCORE=`grep "$NET: " NN/$f/scores 2>/dev/null|sed "s/$NET: //"`
		echo -e "net: $NET  \tkw: $KW\tScore: $SCORE"
	done
	echo Avg KingWeight: `echo "$SUM / 30" | bc -l`
done