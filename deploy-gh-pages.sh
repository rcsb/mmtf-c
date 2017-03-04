#!/bin/bash

version=$1

git checkout gh-pages
rm -r docs/api/$version/*
git add .
git commit -m "old documentation is removed for "$version
git push
git checkout master

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

