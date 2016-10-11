#!/bin/sh

gcc -g -O0 -c mmtf_parser.c &&
gcc -g -O0 -c demo.c &&

gcc -g -O0 -o a.run mmtf_parser.o demo.o -lmsgpackc &&

rm *.o

