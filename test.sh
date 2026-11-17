#!/bin/bash

run_test() {
    name=$1
    seq=$2
    multi=$3
    timing_method=$4

    echo "===== $name ====="

    echo "Sequential:"
    if [ "$timing_method" = "printed" ]; then
        seq_output=$(./"$seq") || return
        seq_time=$(printf '%s\n' "$seq_output" | awk '/^Execution time:/ {print $3; exit}')
    else
        seq_time=$( { /usr/bin/time -f "%e" ./"$seq" > /dev/null; } 2>&1 )
    fi
    echo "Time: ${seq_time}s"

    echo "Multi:"
    if [ "$timing_method" = "printed" ]; then
        multi_output=$(./"$multi") || return
        multi_time=$(printf '%s\n' "$multi_output" | awk '/^Execution time:/ {print $3; exit}')
    else
        multi_time=$( { /usr/bin/time -f "%e" ./"$multi" > /dev/null; } 2>&1 )
    fi
    echo "Time: ${multi_time}s"

    speedup=$(awk "BEGIN {printf \"%.2f\", $seq_time / $multi_time}")
    echo "Speedup: ${speedup}x"
    echo
}

run_test "Gaussian" gaussianseq gaussianmulti time
run_test "Sort" sortseq sortmulti printed