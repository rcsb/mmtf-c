#!/bin/sh

gcc -c mmtf_parser.c &&
gcc -c multi.c &&

gcc -o a.run mmtf_parser.o multi.o -lmsgpackc &&

rm *.o &&
./a.run
