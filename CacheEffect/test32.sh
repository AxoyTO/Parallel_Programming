#!/bin/bash

./bin 100 d
echo "32-Bit Test1: N=100 T='d'"
./main a b c 0
./main a b c 1
./main a b c 2
./main a b c 3
./main a b c 4
./main a b c 5

./bin 500 d
echo "32-Bit Test 2: N=500 T='d'"
./main a b c 0
./main a b c 1
./main a b c 2
./main a b c 3
./main a b c 4
./main a b c 5

./bin 3000 d
echo "32-Bit Test 3: N=3000 T='d'"
./main a b c 0
./main a b c 1
./main a b c 2
./main a b c 3
./main a b c 4
./main a b c 5

