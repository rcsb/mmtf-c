#!/bin/sh

FILES="$(pwd)/data/*.mmtf"

for mmtf in $FILES

do
    echo "Reading file $mmtf"
    ./demo.exe $mmtf
done
