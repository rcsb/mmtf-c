#!/bin/sh

gcc -c array_converters.c &&
gcc -c array_decoders.c &&
gcc -c decoders.c &&
gcc -c demo.c &&

gcc -o a.run array_converters.o array_decoders.o decoders.o demo.o -lmsgpackc &&

rm *.o &&

./a.run

