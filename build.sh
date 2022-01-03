#!/bin/bash
set -e
set -x
if [ ! -d "build" ]; then
  mkdir -p build
fi

cd build
cmake -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
  ..
make -j16
