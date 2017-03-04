#!/bin/bash

version=$1

git checkout master  docs/api/$version
git add docs/api/$version
git commit -m "documentation is updated to "$version -- docs/api/$version
git push origin master:gh-pages
git checkout master
