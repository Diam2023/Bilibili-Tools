#!/bin/sh

mkdir -p build
cd build || (echo "Error" && exit)
cmake -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=$PWD/out \
  -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=$PWD/out \
  -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=$PWD/out ..

make -j10

cd ..
