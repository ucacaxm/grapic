#!/bin/bash

pause()
{
#	read -p "Press [Enter] to continue..."
	printf "Continue ...\n"
}


APPS="apps"
#GRAPIC="grapic.github"
GRAPIC=`pwd`
DOWN="${GRAPIC}/.."
VERSION=`cat doc/VERSION`
SAVE_SHORT="grapic-${VERSION}"
SAVE="${GRAPIC}/../${SAVE_SHORT}"
TOCOPY="apps bin build data script extern src Makefile grapic.lua premake* README.md"

TOZIP_SHORT="apps data src Makefile grapic.lua premake4.lua README.md"
TOZIP_LINUX_SHORT="script/premake4.linux script/premake5.linux build/linux"
TOZIP_MINGW_SHORT="extern/mingw bin/*.dll script/premake4.exe script/premake5.exe build/windows premake.bat"
TOZIP_VISUAL2015_SHORT="extern/visual2015 premake-visual.bat script/premake5.exe script/premake4.exe build/windows"
TOZIP_MAC_SHORT="extern/macosx script/premake4.macosx script/premake5.macosx build/macosx"

TOZIP=`for i in ${TOZIP_SHORT};do printf "${SAVE_SHORT}/$i ";done`
TOZIP_LINUX=`for i in ${TOZIP_LINUX_SHORT};do printf "${SAVE_SHORT}/$i ";done`
TOZIP_MINGW=`for i in ${TOZIP_MINGW_SHORT};do printf "${SAVE_SHORT}/$i ";done`
TOZIP_MAC=`for i in ${TOZIP_MAC_SHORT};do printf "${SAVE_SHORT}/$i ";done`
TOZIP_VISUAL2015=`for i in ${TOZIP_VISUAL2015_SHORT};do printf "${SAVE_SHORT}/$i ";done`


echo "GRAPIC=${GRAPIC}"
echo "GRAPIC-ZIP=${SAVE}"
echo "TOZIP_SHORT=${TOZIP_SHORT}"
echo "TOZIP=${TOZIP}"
echo "TOZIP_LINUX=${TOZIP_LINUX}"
echo "TOZIP_MINGW=${TOZIP_MINGW}"
echo "TOZIP_MAC=${TOZIP_MAC}"
echo "TOZIP_VISUAL2015=${TOZIP_VISUAL2015}"

check()
{
	for arg
	do
		# faire quelque chose avec $arg
		if [ ! -e "${DOWN}/$arg" ]; then
			echo "File ${DOWN}/$arg not found! ERREUR"
			exit 1
		fi
	done
}

pause
#ZIP="/c/Program\ Files/7-Zip/7z.exe"
ZIP="zip"

echo "=============================> CREATE COPY WITH VERSION: ${SAVE}"
cd ${GRAPIC}
make bin/remove_correction.exe
rm -rf ${SAVE}
mkdir ${SAVE}
cp -rf ${TOCOPY} ${SAVE} 
check ${TOZIP}
check ${TOZIP_MAC}
check ${TOZIP_LINUX}
check ${TOZIP_MINGW}
check ${TOZIP_VISUAL2015}

pause 

echo "=============================> REMOVE CORRECTION: ${APPS}"
cd ${SAVE}
find apps -name "*.cpp" -exec ${GRAPIC}/bin/remove_correction.exe {} {} \;





		
makezip () 
{
	cd ${SAVE}/..
	rm -f $1
	echo "In `pwd`"
	RUN="${ZIP} -q -r $1 $2 $3"
	echo $RUN
	eval ${RUN}
	cd ${GRAPIC}
	pause
}

echo "=============================> MINGW win"
FILE="${GRAPIC}/doc/download/grapic-${VERSION}_win.zip"
makezip "${FILE}" "${TOZIP}" "${TOZIP_MINGW}"
echo "=============================> ZIP mac"
FILE="${GRAPIC}/doc/download/grapic-${VERSION}_mac.zip"
makezip "${FILE}" "${TOZIP}" "${TOZIP_MAC}"
echo "=============================> ZIP VISUAL2015"
FILE="${GRAPIC}/doc/download/grapic-${VERSION}_visual2015.zip"
makezip "${FILE}" "${TOZIP}" "${TOZIP_VISUAL2015}"
echo "=============================> LINUX TGZ"
FILETGZ="${GRAPIC}/doc/download/grapic-${VERSION}.tgz"
cd ${SAVE}/..
rm -f ${FILETGZ}
tar cfz ${FILETGZ} ${TOZIP} ${TOZIP_LINUX}
cd ${GRAPIC}

pause

cd ${GRAPIC}
rm -rf ${SAVE}
