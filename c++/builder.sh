#!/bin/bash

set -e
echo "Building SDL3 for use"
cmake -S SDL -B core/include/SDL/build
cmake --build core/include/SDL/build
echo "Building C++ engine..."
cmake -S core -B core/build
cmake --build core/build

echo "Done."
