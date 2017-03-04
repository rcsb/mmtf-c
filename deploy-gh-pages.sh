#!/bin/bash

version=$1

git checkout gh-pages
if [ -d "docs/api/"$version ]; then rm -Rf docs/api/$version; fi
git checkout master  docs/api/$version
git add docs/api/$version
git commit -m "documentation is updated to "$version -- docs/api/$version
git push origin gh-pages
git checkout master
