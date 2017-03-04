#!/bin/bash

version=$1

cd docs
if [ -d "api/"$version ]; then rm -Rf api/$version; fi
doxygen Doxyfile
cd ..
