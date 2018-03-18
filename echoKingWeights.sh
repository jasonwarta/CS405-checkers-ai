#!/usr/bin/env bash

for f in `ls NN`;do 
	echo "$f"
	for g in `ls NN/$f/nets`;do
		w="$(cat NN/$f/nets/$g|grep -i 'kw'|sed 's/kw/kw:/' )"
		echo -e "net: $g  \t$w"
	done
done