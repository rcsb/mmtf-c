#!/bin/bash

if [ "$TRAVIS_REPO_SLUG" == "rcsb/mmtf-c" ] && [ "$TRAVIS_PULL_REQUEST" == "false" ] && [ "$TRAVIS_BRANCH" == "master" ]; then

  echo -e "Publishing documentation...\n"

  cp -R doc/api/v.0.0.1 $HOME/docs-latest

  cd $HOME
  git config --global user.email "travis@travis-ci.org"
  git config --global user.name "travis-ci"
  git clone --quiet --branch=gh-pages https://${GH_TOKEN}@github.com/rcsb/mmtf-c gh-pages > /dev/null

  cd gh-pages
  git rm -rf ./doc
  cp -Rf $HOME/docs-latest ./doc
  git add -f .
  git commit -m "Latest documentation on successful travis build $TRAVIS_BUILD_NUMBER auto-pushed to gh-pages"
  git push -fq origin gh-pages > /dev/null

  echo -e "Published documentation to gh-pages.\n"
  
fi
