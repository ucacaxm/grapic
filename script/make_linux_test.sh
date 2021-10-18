#!/bin/bash

pwd
VERSION=`cat doc/VERSION`
mkdir -p tmp
cd tmp
tar xfvz ../doc/download/grapic-${VERSION}-linux.tgz
cd grapic-${VERSION}-linux
make





