#!/bin/bash

tmp=/tmp/$$
mkdir $tmp
tag=`git tag | tail -n 1`
out=`pwd`
cd $tmp
git clone https://github.com/cth103/plugin-torture.git
cd plugin-torture
git checkout $tag
cd ..
mv plugin-torture plugin-torture-$tag
tar -c plugin-torture-$tag --exclude ".git" | bzip2 -f -9 > $out/plugin-torture-$tag.tar.bz2
