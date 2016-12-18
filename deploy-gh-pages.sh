#!/bin/bash

version=$1
echo "Update to "$version
git checkout gh-pages
if [ -d "docs/api/"$version ]; then rm -Rf docs/api/$version; fi
git checkout master —- docs/api/$version
git add .
git commit -m "documentation is updated"
git push
git checkout master
