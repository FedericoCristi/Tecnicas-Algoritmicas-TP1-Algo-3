#!/bin/bash

cd /data || exit 1
mkdir /data/tmp
mv cmake_install.cmake build.ninja CMakeCache.txt /data/tmp
cmake .
make
rm Makefile .ninja_log .ninja_deps
mv /data/tmp/* /data/
rmdir /data/tmp
tail -f /dev/null