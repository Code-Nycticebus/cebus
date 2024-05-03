#!/bin/sh

INSTALL_DIRECTORY="$(pwd)/lib"

mkdir -p "$INSTALL_DIRECTORY"
mkdir -p "$INSTALL_DIRECTORY"/include

set -e

for f in src/clib/**/*.c
do 
  gcc -I./src -c "$f" 
done

ar rcs lib/libclib.a *.o 
rm *.o

cp src/clib.h lib/include
cp -r src/clib/ lib/include 
for f in lib/include/clib/**/*.c
do 
  rm $f
done
