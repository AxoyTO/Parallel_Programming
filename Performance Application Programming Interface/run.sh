#!/bin/bash

./bin 1024 d
echo "32-Bit Test 1: N=1024 T='d'"
./main a b c 0
./main a b c 1
./main a b c 2
./main a b c 3
./main a b c 4
./main a b c 5

./bin 1024 l
echo "64-Bit Test 2: N=1024 T='l'"
./main a b c 0
./main a b c 1
./main a b c 2
./main a b c 3
./main a b c 4
./main a b c 5

./bin 2048 d
echo "32-Bit Test 3: N=2048 T='d'"
./main a b c 0
./main a b c 1
./main a b c 2
./main a b c 3
./main a b c 4
./main a b c 5

./bin 2048 l
echo "64-Bit Test 4: N=2048 T='l'"
./main a b c 0
./main a b c 1
./main a b c 2
./main a b c 3
./main a b c 4
./main a b c 5
