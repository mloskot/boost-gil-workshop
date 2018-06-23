#!/bin/bash
set -ex
rm -rf a.out a.bmp b.bmp
UBSAN_OPTIONS='print_stacktrace=1'
#-fno-sanitize-recover=integer
#-fsanitize-blacklist=libs/gil/.ci/blacklist.supp
$CC -std=c++11 -Wno-unused -g -fstrict-aliasing -fno-omit-frame-pointer -fsanitize=integer -I/mnt/d/boost.wsl $*
file ./a.out
./a.out
