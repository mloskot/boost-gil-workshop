#!/bin/bash
set -ex
rm -rf a.out a.bmp b.bmp
UBSAN_OPTIONS='print_stacktrace=1'
$CC -g -std=c++11 -Wno-unused -fstrict-aliasing -fno-omit-frame-pointer -fsanitize=undefined -I/mnt/d/boost.wsl $*
./a.out
file ./a.out
./a.out
