#!/bin/bash

for i in $(seq 1 6)
do
    FN="data/words-$i"
    EXACT_COUNT=`cat $FN | tr ' ' '\n' | sort -u  | wc -l`
    CARD_EST=`cat $FN | ./ccard`
    DIFF=`echo "100 - (100 * $CARD_EST/$EXACT_COUNT)" | bc`
    if [ "$DIFF" -lt -5 ] || [ "$DIFF" -gt 5 ]
    then
        echo "Failure with $FN exact=$EXACT_COUNT estimate=$CARD_EST (diff=$DIFF%)"
        exit 1
    fi
done
