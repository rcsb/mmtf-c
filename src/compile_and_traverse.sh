#!/bin/sh

gcc -c mmtf_parser.c &&
gcc -c traverse.c &&

gcc -o traverse.exe mmtf_parser.o traverse.o -lmsgpackc &&

rm *.o

./traverse.exe $1
