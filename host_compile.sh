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
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
  ..
cmake --build ${BUILD_DIR} -j16

# judge and create .local folder, do install
# make DESTDIR=${INSTALL_DIR} install
# -DPYTHON_EXECUTABLE=`which python3` \
