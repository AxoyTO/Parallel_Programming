#!/bin/bash

echo "-------------  RUN #1 -------------"
echo " *  ARRAY SIZE: 1000000\n *  THREADS: 1"
./SortOMP 1000000 1
echo ""

echo "-------------  RUN #2  -------------"
echo " *  ARRAY SIZE: 1000000\n *  THREADS: 2"
./SortOMP 1000000 2
echo ""

echo "-------------  RUN #3  -------------"
echo " *  ARRAY SIZE: 1000000\n *  THREADS: 4"
./SortOMP 1000000 4
echo ""

echo "-------------  RUN #4  -------------"
echo " *  ARRAY SIZE: 1000000\n *  THREADS: 8"
./SortOMP 1000000 8
echo ""


echo "-------------  RUN #5  -------------"
echo " *  ARRAY SIZE: 1000000\n *  THREADS: 16"
./SortOMP 1000000 16
echo ""

echo "-------------  RUN #6  -------------"
echo " *  ARRAY SIZE: 1000000\n *  THREADS: 32"
./SortOMP 1000000 32
echo ""


echo "-------------  RUN #7  -------------"
echo " *  ARRAY SIZE: 1000000\n *  THREADS: 64"
./SortOMP 1000000 64
echo ""

# in POLUS(POWR8) -> 10 cores in each socket. 8 threads per core. -> 80 threads per socket
echo "-------------  RUN #8  -------------"
echo " *  ARRAY SIZE: 1000000\n *  THREADS: 80"
./SortOMP 1000000 80
echo ""

: 'ARRAY SIZE: 2000000 -> SEGMENTATION FAULT IN POLUS...
echo "\n *  ARRAY SIZE: 2000000\n *  THREADS: 12"
./SortOMP 2000000 12

echo "\n *  ARRAY SIZE: 1000000\n *  THREADS: 24"
./SortOMP 2000000 24

echo "\n *  ARRAY SIZE: 2000000\n *  THREADS: 48"
./SortOMP 2000000 48

echo "\n *  ARRAY SIZE: 2000000\n *  THREADS: 72"
./SortOMP 2000000 72

echo "\n *  ARRAY SIZE: 2000000\n *  THREADS: 96"
./SortOMP 2000000 96

echo "\n *  ARRAY SIZE: 2000000\n *  THREADS: 128"
./SortOMP 2000000 128
'