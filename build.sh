#!/usr/bin/env bash

set -euo pipefail

cmake -S . -B build -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles"
cmake --build build --target install --parallel