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
  -DBUILD_SHARED_LIBS=ON \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
  -DCMAKE_TOOLCHAIN_FILE=./cmake/aarch64-linux-gnu.toolchain.cmake \
  ..
cmake --build ${BUILD_DIR} -j16

# judge and create .local folder, do install
# -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
# make DESTDIR=${INSTALL_DIR} install
