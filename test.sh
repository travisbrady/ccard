#!/bin/bash

for i in $(seq 1 6)
do
    FN="data/words-$i"
    EXACT_COUNT=`gunzip -c $FN | tr ' ' '\n' | sort -u  | wc -l`
    CARD_EST=`gunzip -c $FN | ./ccard`
    DIFF=`echo "100 - (100 * $CARD_EST/$EXACT_COUNT)" | bc`
    DIFF_FLOAT=`echo "scale=3;100 - (100 * $CARD_EST/$EXACT_COUNT)" | bc -l`
    echo "file: $FN"
    echo "exact: $EXACT_COUNT"
    echo "estimate: $CARD_EST"
    printf "ratio: %.3f%%\n\n" $DIFF_FLOAT
    if [ "$DIFF" -lt -3 ] || [ "$DIFF" -gt 3 ]
    then
        echo "Failure with $FN exact=$EXACT_COUNT estimate=$CARD_EST (diff=$DIFF%)"
        exit 1
    fi
done
