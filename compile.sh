#!/bin/bash
set -e
set -x
SOUCE_DIR=./
BUILD_DIR=${SOUCE_DIR}/build/
INSTALL_DIR=${BUILD_DIR}/install/

cmake \
  -S ${SOUCE_DIR} \
  -B ${BUILD_DIR} \
  -G "Unix Makefiles" \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
  ..
cmake --build ${BUILD_DIR} -j2

