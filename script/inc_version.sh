cd script
export A=`cat ../doc/VERSION | cut -d '.' -f 1`
export B=`cat ../doc/VERSION | cut -d '.' -f 2`
export C=`cat ../doc/VERSION | cut -d '.' -f 3`
export C=$((C+1))
echo "new version: $A.$B.$C"
echo "$A.$B.$C"  > ../doc/VERSION

echo "" > ../doc/VERSION.html

echo "<UL>" >> ../doc/VERSION.html
echo "<LI> <a href=\"../download/grapic-$A.$B.${C}_win.zip\"> grapic-$A.$B.${C}_win.zip (WINDOWS/Codeblocks/Mingw) </a>"  >> ../doc/VERSION.html
echo "<LI> <a href=\"../download/grapic-$A.$B.${C}_mac.zip\"> grapic-$A.$B.${C}_mac.zip (MACOS)</a>"  >> ../doc/VERSION.html
echo "<LI> <a href=\"../download/grapic-$A.$B.$C.tgz\"> grapic-$A.$B.$C.tgz (LINUX) </a>"  >> ../doc/VERSION.html
echo "<LI> <a href=\"../download/grapic-$A.$B.${C}_visual2015.zip\"> grapic-$A.$B.${C}_visual2015.zip (WINDOWS/VisualStudio2015) </a>"  >> ../doc/VERSION.html
echo "</UL>" >> ../doc/VERSION.html
