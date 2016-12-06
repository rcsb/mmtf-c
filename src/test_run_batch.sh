#!/bin/sh

FILES="$(pwd)/data/*.mmtf"

for mmtf in $FILES

do
    echo "Reading file $mmtf"
    ./traverse.exe $mmtf
done
