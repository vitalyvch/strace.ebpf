#!/bin/bash

#set -x

TEST=2

for ((i=0; i<100; i++)); do
	./test-trace_py ./test_basic $TEST

	qty=$(grep -w sys_poll cut-${TEST}.log | wc -l | awk '{ print $1 }')

	if [ "2" != "$qty" ]; then
		echo "$TEST:i=$i: qty = \"$qty\""

		mv cut-${TEST}.log cut-${TEST}.${qty}_$i.log
	fi
done
