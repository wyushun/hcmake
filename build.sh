#!/bin/bash
set -e
set -x
if [ ! -d "build" ]; then
  mkdir -p build
fi

cd build
cmake -G "Unix Makefiles" \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
  ..
cmake --build . -j16
