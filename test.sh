#!/bin/bash

run_test() {
    name=$1
    seq=$2
    multi=$3

    echo "===== $name ====="

    echo "Sequential:"
    seq_time=$( { /usr/bin/time -f "%e" ./"$seq" > /dev/null; } 2>&1 )
    echo "Time: ${seq_time}s"

    echo "Multi:"
    multi_time=$( { /usr/bin/time -f "%e" ./"$multi" > /dev/null; } 2>&1 )
    echo "Time: ${multi_time}s"

    speedup=$(awk "BEGIN {printf \"%.2f\", $seq_time / $multi_time}")
    echo "Speedup: ${speedup}x"
    echo
}

run_test "Gaussian" gaussianseq gaussianmulti
run_test "Sort" sortseq sortmulti