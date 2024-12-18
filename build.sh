#!/bin/sh

set -xe

CC=${CC:=/usr/bin/cc}

CFLAGS="-Wall -Wextra -Wswitch-enum -Wmissing-prototypes -Wconversion -fno-strict-aliasing -std=c11 -pedantic"
LIBS=
 
$CC $CFLAGS -o main main.c $LIBS