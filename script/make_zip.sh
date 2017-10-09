#! /bin/sh  

APPS="apps"
SAVE="${APPS}.remove_correction"
GRAPIC="grapic.git"

ZIP="/c/Program\ Files/7-Zip/7z.exe"

echo "=============================> REMOVE CORRECTION: ${APPS}"
cp -r $APPS $SAVE
find ${SAVE} -name "*.cpp" -exec bin/remove_correction.exe {} {} \;



REMOVE="-x!${GRAPIC}/.git \
		-x!${GRAPIC}/obj \
		-x!${GRAPIC}/build-cmake \
		-x!${GRAPIC}/doc -x!${GRAPIC}/doc/download -x!${GRAPIC}/doc/html -x!${GRAPIC}/doc/images/old  \
		-x!${GRAPIC}/bin/*.exe \
		-x!${GRAPIC}/tmp \
		-x!${GRAPIC}/${APPS} \
		-x!${GRAPIC}/${SAVE}/zzz* \
		-x!${GRAPIC}/data/zzz* \
		-x!${GRAPIC}/premake-beta.bat \
		-x!${GRAPIC}/extern/visual2013 \
		-x!${GRAPIC}/bin/*.ilk \
		"

REMOVE_MINGW="-x!${GRAPIC}/extern/mingw"
REMOVE_MAC="-x!${GRAPIC}/extern/macosx"
REMOVE_VISUAL2015="-x!${GRAPIC}/extern/visual2015 -x!${GRAPIC}/premake-visual.bat -x!${GRAPIC}/script/premake5.*"
#					-x!${GRAPIC}/script/premake5.linux -x!${GRAPIC}/script/premake5.exe -x!${GRAPIC}/script/premake5.macosx"

		
makezip () 
{
	eval ${ZIP} a $1 ../${GRAPIC} ${REMOVE} $2 $3 $4
										
	# RENAME FILE INSIDE THE ARCHIVE
	eval ${ZIP} rn $1 ${GRAPIC}/${SAVE} ${GRAPIC}/${APPS}
	eval ${ZIP} rn $1 ${GRAPIC} grapic-`cat doc/VERSION`
}

echo "=============================> MINGW win"
FILE="doc/download/grapic-`cat doc/VERSION`_win.zip"
makezip "${FILE}" "${REMOVE_MAC}" "${REMOVE_VISUAL2015}" ""
echo "=============================> ZIP mac"
FILE="doc/download/grapic-`cat doc/VERSION`_mac.zip"
makezip "${FILE}" "${REMOVE_MINGW}" "${REMOVE_VISUAL2015}" ""
echo "=============================> ZIP VISUAL2015"
FILE="doc/download/grapic-`cat doc/VERSION`_visual2015.zip"
makezip "${FILE}" "${REMOVE_MINGW}" "${REMOVE_MAC}" ""
echo "=============================> TGZ"
FILETAR="doc/download/grapic-`cat doc/VERSION`.tar"
FILETGZ="doc/download/grapic-`cat doc/VERSION`.tgz"
makezip "${FILETAR}" "${REMOVE_MINGW}" "${REMOVE_MAC}" "${REMOVE_VISUAL2015}"
eval ${ZIP} a ${FILETGZ} ${FILETAR}
rm -rf ${FILETAR}

rm -rf tmp/${SAVE}
mv $SAVE tmp
