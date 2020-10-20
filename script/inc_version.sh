#!/bin/bash
echo "########### inc_version.sh"
pwd
cd script
export A=`cat ../doc/VERSION | cut -d '.' -f 1`
export B=`cat ../doc/VERSION | cut -d '.' -f 2`
export C=`cat ../doc/VERSION | cut -d '.' -f 3`
echo "old version: $A $B $C"
export C=`expr $C + 1`
echo "new version: $A.$B.$C"
echo "$A.$B.$C"  > ../doc/VERSION

echo "" > ../doc/VERSION.html

echo "<UL>" >> ../doc/VERSION.html
echo "<LI> <a href=\"../download/grapic-$A.$B.${C}-WinCB20.zip\"> grapic-${A}.${B}.${C}-WinCB20.zip (WINDOWS/Codeblocks-20.03/Mingw) </a>"  >> ../doc/VERSION.html
echo "<LI> <a href=\"../download/grapic-$A.$B.${C}-MacOS.zip\"> grapic-${A}.${B}.${C}_MacOS.zip (MACOS)</a>"  >> ../doc/VERSION.html
echo "<LI> <a href=\"../download/grapic-$A.$B.$C-Linux.tgz\"> grapic-${A}.${B}.${C}-Linux.tgz (LINUX) </a>"  >> ../doc/VERSION.html
echo "<LI> <a href=\"../download/grapic-$A.$B.${C}-WinVS2015.zip\"> grapic-${A}.${B}.${C}_WinVS2015.zip (WINDOWS/VisualStudio2015) </a>"  >> ../doc/VERSION.html
echo "<LI> <a href=\"../download/grapic-$A.$B.${C}-WinCB17.zip\"> grapic-${A}.${B}.${C}-WinCB17.zip (WINDOWS/Codeblocks-17.12/Mingw) </a>"  >> ../doc/VERSION.html
echo "</UL>" >> ../doc/VERSION.html

echo "########### inc_version.sh: done"
