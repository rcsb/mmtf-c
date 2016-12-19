#!/bin/bash

version=$1

cd docs
if [ -d "api/"$version ]; then rm -Rf api/$version; fi
doxygen Doxyfile
cd ..
git add . 
git commit –m “documentation is generated for ”$version
git push

git checkout gh-pages
rm -r docs/api/$version/*
git checkout master — docs/api/$version
git add .
git commit “documentation is updated to ”$version
git push
git checkout master
