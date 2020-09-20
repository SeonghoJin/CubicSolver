#!/bin/bash
root="/home/work/seongho"

echo "..cd ${root}/src"
cd ${root}/src

echo "..mkdir build"
cd ../
mkdir build -p
cd build

echo "..cmake ../src"
cmake ../src

echo "..make"
make
cd ../src

echo "..run"
../bin/CubicSolverTest