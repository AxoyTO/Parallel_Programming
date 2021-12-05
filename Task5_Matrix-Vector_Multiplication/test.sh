#!/bin/bash

ulimit -s 10240
echo "----------------------------"
./binary m 4096
./binary v 4096
echo "=============="
echo "Test 1: N=4096"
echo "=============="
mpiexec -n 1 main a b c
mpiexec -n 2 main a b c
mpiexec -n 4 main a b c
mpiexec -n 8 main a b c
mpiexec -n 16 main a b c
echo "----------------------------"
echo "----------------------------"
./binary m 8192
./binary v 8192
echo "=============="
echo "Test 2: N=8192"
echo "=============="
mpiexec -n 1 main a b c
mpiexec -n 2 main a b c
mpiexec -n 4 main a b c
mpiexec -n 8 main a b c
mpiexec -n 16 main a b c
echo "----------------------------"
