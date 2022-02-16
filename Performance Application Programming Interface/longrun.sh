#!/bin/bash

./bin 4096 d
echo "32-Bit Test 1: N=4096 T='d'"
./main a b c 0
./main a b c 1
./main a b c 2
./main a b c 3
./main a b c 4
./main a b c 5

./bin 4096 l
echo "64-Bit Test 2: N=4096 T='l'"
./main a b c 0
./main a b c 1
./main a b c 2
./main a b c 3
./main a b c 4
./main a b c 5
