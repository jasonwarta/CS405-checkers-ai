#!/usr/bin/env bash

NN_DIR="NN"
POP_SIZE=`grep POPULATION_SIZE src/consts.h|sed 's/.*= \?\([0-9]*\);/\1/'`
LS_CMD="ls -tr $NN_DIR"

for i in "$@"; do
    case $i in
        -b|--brief)
        LS_CMD="ls -tr \"$NN_DIR\" | tail"
        shift # past argument=value
        ;;
    esac
done

for f in $(echo "$LS_CMD"|bash);do 
	echo "`sed 's/gen_/\nGeneration: /'<<<$f`"
	SUM=0.0
	for NET in `ls "$NN_DIR"/$f/nets`;do
		KW="$(cat "$NN_DIR"/$f/nets/$NET|grep -i 'kw'|sed 's/kw //' )"
		SUM=`echo $SUM + $KW | bc`
		SCORE=`grep "$NET: " "$NN_DIR"/$f/scores 2>/dev/null|sed "s/$NET: //"`
		if [ -z $SCORE ]; then SCORE="NAN"; fi
		LAST_WEIGHT=$(sed -E 's/edges.* ([0-9\.]{2,9}) $/\1/' <<< `cat NN/$f/nets/$NET|grep edges`)
		echo -e "net: $NET  \tkw: $KW\tScore: $SCORE\tlast edge:\t$LAST_WEIGHT"
	done
	echo Avg KingWeight: `echo "$SUM / $POP_SIZE" | bc -l`
done