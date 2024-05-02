#/bin/bash
gcc -I./src -c ./src/**/*.c 
ar rcs libclib.a *.o 
rm *.o