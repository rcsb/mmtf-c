#!/bin/bash

version=$1

cd docs
if [ -d "api/"$version ]; then rm -Rf api/$version; fi
doxygen Doxyfile
git add api/$version
git commit –m "documentation is generated for "$version -- api/$version
git push origin master
cd ..

git checkout gh-pages
rm -r docs/api/$version/*
git checkout master — docs/api/$version
git add docs/api/$version
git commit “documentation is updated to ”$version -- docs/api/$version
git push origin gh-pages
git checkout master
