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

git checkout gh-pages
git checkout master  docs/api/$version
git add docs/api/$version
git commit -m "documentation is updated to "$version -- docs/api/$version
git push origin gh-pages
git checkout master

rm -r docs
