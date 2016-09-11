#!/bin/sh

gcc array_converters.c &&
gcc array_decoders.c &&
gcc decoders.c &&
gcc demo.c &&
gcc mmtf_structure.c &&
gcc num_converters.c &&

gcc -o a.run array_converters.o array_decoders.o decoders.o demo.o mmtf_structure.o num_converters.c

rm *.o

