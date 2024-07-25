#!/bin/sh

cd build \
&&
cmake .. \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DSTATIC_STDCXX=true \
&&
cmake \
    --build . \
    --config RelWithDebInfo \
    --parallel $(nproc)
