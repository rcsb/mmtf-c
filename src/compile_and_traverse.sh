#!/bin/sh

gcc -c mmtf_parser.c &&
gcc -c traverse.c &&

gcc -o a.run mmtf_parser.o traverse.o -lmsgpackc &&

rm *.o

#./a.run $1
