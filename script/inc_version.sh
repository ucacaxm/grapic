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
mv ../doc/VERSION ../doc/VERSION.old
echo "$A.$B.$C"  > ../doc/VERSION

echo "" > ../doc/VERSION.html

echo "<UL>" >> ../doc/VERSION.html
echo "<LI> <a href=\"../download/grapic-$A.$B.${C}-wincb20.zip\"> grapic-${A}.${B}.${C}-wincb20.zip (WINDOWS/Codeblocks-20.03/Mingw) </a>"  >> ../doc/VERSION.html
echo "<LI> <a href=\"../download/grapic-$A.$B.${C}-macosx.zip\"> grapic-${A}.${B}.${C}_macosx.zip (MACOS)</a>"  >> ../doc/VERSION.html
echo "<LI> <a href=\"../download/grapic-$A.$B.$C-linux.tgz\"> grapic-${A}.${B}.${C}-linux.tgz (LINUX) </a>. You need to install librairies, see 'Run on Linux'"  >> ../doc/VERSION.html
echo "<LI> <a href=\"../download/grapic-$A.$B.${C}-winvs2019.zip\"> grapic-${A}.${B}.${C}_winvs2019.zip (WINDOWS/VisualStudio2019) </a>."  >> ../doc/VERSION.html
echo "<LI> <a href=\"../download/grapic-$A.$B.${C}-wincb17.zip\"> grapic-${A}.${B}.${C}-wincb17.zip (WINDOWS/Codeblocks-17.12/Mingw) </a>.  For the previous version of Codeblocks but we recommand to upgrade." >> ../doc/VERSION.html
echo "</UL>" >> ../doc/VERSION.html

echo "########### inc_version.sh: done"
