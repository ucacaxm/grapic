#!/bin/bash

APPS="apps"
GRAPIC="grapic.github"
VERSION=`cat doc/VERSION`
SAVE="../grapic-${VERSION}"
TOCOPY="apps bin build data script extern src Makefile premake4.lua README.md"

#ZIP="/c/Program\ Files/7-Zip/7z.exe"
ZIP="zip"

echo "=============================> CREATE COPY WITH VERSION: ${SAVE}"
make clean
mkdir ../${SAVE}
cp -rf ${TOCOPY} ${SAVE} 

echo "=============================> REMOVE CORRECTION: ${APPS}"
cd ${SAVE}
find apps -name "*.cpp" -exec bin/remove_correction.exe {} {} \;



REMOVE="-x ${GRAPIC}/.git\* \
		-x ${GRAPIC}/obj\* \
		-x ${GRAPIC}/build-cmake\* \
		-x ${GRAPIC}/doc\* \
		-x ${GRAPIC}/doc/download\* \
		-x ${GRAPIC}/doc/html\* \
		-x ${GRAPIC}/doc/images/old\*  \
		-x ${GRAPIC}/bin/\*.exe \
		-x ${GRAPIC}/tmp\* \
		-x ${GRAPIC}/apps/zzz\** \
		-x ${GRAPIC}/data/zzz\** \
		-x ${GRAPIC}/premake-beta.bat \
		-x ${GRAPIC}/extern/visual2013\* \
		-x ${GRAPIC}/bin/*.ilk \
		"

REMOVE_MINGW="-x ${GRAPIC}/extern/mingw\*"
REMOVE_MAC="-x ${GRAPIC}/extern/macosx\*"
REMOVE_VISUAL2015="-x ${GRAPIC}/extern/visual2015\* \
					-x ${GRAPIC}/premake-visual.bat \
					-x ${GRAPIC}/script/premake5.\*"

		
makezip () 
{
	#pwd
	#echo "==> ${ZIP} a $1 ../${GRAPIC}"
	cd ${SAVE}/..
	eval ${ZIP} -r $1 ${SAVE} ${REMOVE} $2 $3 $4
										
	
	cd ${GRAPIC}
}

echo "=============================> MINGW win"
FILE="${GRAPIC}/doc/download/grapic-${VERSION}_win.zip"
makezip "${FILE}" "${REMOVE_MAC}" "${REMOVE_VISUAL2015}" ""
echo "=============================> ZIP mac"
FILE="${GRAPIC}/doc/download/grapic-${VERSION}_mac.zip"
makezip "${FILE}" "${REMOVE_MINGW}" "${REMOVE_VISUAL2015}" ""
echo "=============================> ZIP VISUAL2015"
FILE="${GRAPIC}/doc/download/grapic-${VERSION}_visual2015.zip"
makezip "${FILE}" "${REMOVE_MINGW}" "${REMOVE_MAC}" ""
echo "=============================> TGZ"
FILETAR="${GRAPIC}/doc/download/grapic-${VERSION}.tar"
FILETGZ="${GRAPIC}/doc/download/grapic-${VERSION}.tgz"
makezip "${FILETAR}" "${REMOVE_MINGW}" "${REMOVE_MAC}" "${REMOVE_VISUAL2015}"
eval ${ZIP} a ${FILETGZ} ${FILETAR}
rm -rf ${FILETAR}

cd ${GRAPIC}
rm -rf ${SAVE}
