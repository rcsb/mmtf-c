#!/bin/sh

gcc -c mmtf_parser.c &&
gcc -c demo.c &&

gcc -o a.run mmtf_parser.o demo.o -lmsgpackc &&

rm *.o &&

./a.run $1

