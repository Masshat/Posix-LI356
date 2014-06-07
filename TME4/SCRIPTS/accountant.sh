#!/bin/bash
#accountant.sh

i=1
while [ $i -lt 8 ]
do
    echo "SIGNAL : "$i
    if [ $i != 9 ] || [ $i != 19 ]
    then
	kill -$i $1
    fi
    i=`expr $i + 1`
done
