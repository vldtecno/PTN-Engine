#!/usr/bin/env sh
set -evx
env | sort

mkdir build || true
cd build/
cmake -Dbuild_examples=ON \
      -Dbuild_tests=OFF \
      -Dgoogle_test_build_dir="" \
      -Dgoogle_test_include_dir="" 
make

