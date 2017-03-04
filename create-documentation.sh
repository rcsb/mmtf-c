#!/bin/bash

version=$1

if [ ! -d "$docs" ]; then
mkdir docs
fi

if [ ! -d "$docs/api" ]; then
mkdir docs/api
fi

if [ -d "docs/api/"$version ]; then rm -Rf docs/api/$version; fi

doxygen Doxyfile
