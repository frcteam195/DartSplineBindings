#!/bin/bash
if [[ $1 == clean ]]; then
    rm -Rf lib/build
    exit 0
fi

mkdir -p lib/build
cd lib/build

# cmake -DMINGW32=1 ..
# find /usr | grep dll$ | grep -v win32 | grep -v 686

if [ ! -f "Makefile" ]; then
    cmake ..
fi

make
