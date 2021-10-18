#!/bin/bash

pwd
VERSION=`cat doc/VERSION`
mkdir -p tmp
cd tmp
tar xfvz ../doc/download/grapic-${VERSION}-linux.tgz
cd grapic-${VERSION}-linux

OS=linux
OSGMAKE=${OS}-gmake
LIST=`ls build/${OSGMAKE}/*.make 2>/dev/null | cut -d '/' -f 3 | cut -d . -f 1`
echo "============== LIST of PROJECT ======================"
echo $LIST


echo "============== make ======================"
pwd
make

echo "============== Test if exec exists ======================"
pwd
echo "==== ls bin"
ls bin
echo "===="
for i in ${LIST}; do 
    if [ -f "bin/${i}" ]; then
        echo "bin/${i}: ok"
    else
        echo "bin/${i}: error"
    fi
done


echo "============== Run demo ======================"
read -p "Test run demo [y/N]" sure && \
    case "$sure" in [yY]) \
        bin/tuto10_Demo;; \
    *) echo "no run";; \
    esac


echo "============== Clean ======================"
read -p "clean tmp [y/N]" cl && \
    case "$cl" in [yY]) \
        echo "clean" ; cd ../..; /bin/rm -rf tmp ;; \
    *) echo "no clean $cl";; \
    esac


