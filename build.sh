#!/bin/sh

CC="gcc"
CFLAGS="-Wall -Wpedantic -ggdb -std=c11"
CDEFINES=""
CINCLUDES=""
CFILES="main.c"
CLIBS="-lX11"

EXEC="main"

echo "--------COMPILING--------"
echo "$CC $CFLAGS $CDEFINES $CINCLUDES $CFILES -o $EXEC $CLIBS"
$CC $CFLAGS $CDEFINES $CINCLUDES $CFILES -o $EXEC $CLIBS

echo "---------RUNNING---------"
echo "./$EXEC"
./$EXEC
