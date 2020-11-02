#!/bin/bash

pause()
{
	read -p "Press [Enter] to continue..."
	printf "Continue ...\n"
}

echo "####################################################################################"""
echo "################ make_zip.sh #######################################################"""
echo "####################################################################################"""
echo "pwd=`pwd`"
mkdir -p doc/download

#APPS="apps"
#GRAPIC="grapic.github"
GRAPIC=`pwd`
DOWN="${GRAPIC}/.."
VERSION=`cat doc/VERSION`

# files copied to the temp $SAVE dir during making the zip file
#TOCOPY="apps bin build data script extern src Makefile grapic.lua premake* README.md lifami-cb17.bat lifami-cb20.bat lifami-cb17.bat lifami-cb20.bat"

# files which will be included in each zip file
TOZIP_SHORT="apps bin data src Makefile grapic.lua premake4.lua README.md"
TOZIP_MINGW20="${TOZIP_SHORT}    extern/mingw-cb20 script premake-cb20.bat lifami-cb20.bat"
TOZIP_MINGW17="${TOZIP_SHORT}    extern/mingw-cb17 script premake-cb17.bat lifami-cb17.bat"
TOZIP_VISUAL2015="${TOZIP_SHORT} extern/visual2015 script premake-vs2015.bat"
TOZIP_LINUX="${TOZIP_SHORT}               script/premake4.linux.sh script/premake5.linux.sh script/premake5.linux"
TOZIP_MACOS="${TOZIP_SHORT} extern/macosx script/premake4.macosx   script/premake5.macosx script/premake4.linux.sh script/premake5.linux.sh script/premake5.linux"

# add $SAVE_SHORT path to the filename of the zip files list
# TOZIP=`for i in ${TOZIP_SHORT};do printf "${SAVE_SHORT}/$i ";done`
# TOZIP_LINUX=`for i in ${TOZIP_LINUX_SHORT};do printf "${SAVE_SHORT}/$i ";done`
# TOZIP_MINGW20=`for i in ${TOZIP_MINGW20_SHORT};do printf "${SAVE_SHORT}/$i ";done`
# TOZIP_MINGW17=`for i in ${TOZIP_MINGW17_SHORT};do printf "${SAVE_SHORT}/$i ";done`
# TOZIP_MAC=`for i in ${TOZIP_MAC_SHORT};do printf "${SAVE_SHORT}/$i ";done`
# TOZIP_VISUAL2015=`for i in ${TOZIP_VISUAL2015_SHORT};do printf "${SAVE_SHORT}/$i ";done`


echo "GRAPIC=${GRAPIC}"
echo "TOZIP_SHORT=${TOZIP_SHORT}"
echo "TOZIP=${TOZIP}"
echo "TOZIP_LINUX=${TOZIP_LINUX}"
echo "TOZIP_MINGW20=${TOZIP_MINGW20}"
echo "TOZIP_MINGW17=${TOZIP_MINGW17}"
echo "TOZIP_MAC=${TOZIP_MAC}"
echo "TOZIP_VISUAL2015=${TOZIP_VISUAL2015}"

#pause

# $1=path
# $2=files list
check()
{
	for file in $2
	do
		# if ficher existe pas 
		if [ ! -e "$1/$file" ]; then
			echo "File $1/$file not found! ERREUR"
			exit 1
		fi
	done
}

ZIP="zip"




# $1 path
remove_correction()
{
	echo "=============================> REMOVE CORRECTION in $1"
	cd ${GRAPIC}
	make bin/remove_correction.exe
	cd $1
	find apps -name "*.cpp" -exec ${GRAPIC}/bin/remove_correction.exe {} {} \;
}


# $1
# $2 
copy()
{
	#echo "files list=$1"
	#echo "dest=$2"
	mkdir -p $2/script
	mkdir -p $2/extern
	#mkdir -p $2/build
	for file in $1
	do
		#echo "cp -rf $file $2/$file"
		if [[ "$file" == *"/"* ]];then
			cp -rf $file $2/$file
		else
			cp -rf $file $2		
		fi
	done
}



# $1=premake command
# $2=extension archive
# $3=command to do zip 
# $4=files list
makezip () 
{
	NAME="grapic-${VERSION}-$1"
	SAVE_DIR="${DOWN}/${NAME}/"
	ZIP_FILE="${GRAPIC}/doc/download/${NAME}.$2"
	echo "=============================> CREATE COPY WITH VERSION: "
	echo "=== NAME=${NAME}"
	echo "=== SAVE_DIR=${SAVE_DIR}"
	echo "=== ZIP_FILE=${ZIP_FILE}"
	echo "=== FILES LIST=$4"
	#pause

	/bin/rm -rf ${SAVE_DIR}
	mkdir ${SAVE_DIR}
	
	cd ${GRAPIC}
	echo "cp -rf $4 ${SAVE_DIR}"
	#cp -rf $4 ${SAVE_DIR}
	copy "$4" ${SAVE_DIR}
	
	cd ${SAVE_DIR}
	make premake-$1
	
	#pause 
	
	check ${SAVE_DIR} "$4"
	remove_correction ${SAVE_DIR}
	
	
	#RUN="${ZIP} -q -r $1 $2 $3"			# -q quiet   -r recurse-path
	cd ${SAVE_DIR}/..
	RUN="$3 $ZIP_FILE ${NAME}"
	echo $RUN
	eval ${RUN}
	cd ${GRAPIC}
	
	# to debug
	rm -rf ${SAVE_DIR}
	#pause
}

ZIP_COMMAND="${ZIP} -q -r"
TGZ_COMMAND="tar cfvz"

make cleanpremake

makezip WinCB20 zip "$ZIP_COMMAND" "${TOZIP_MINGW20}"

makezip WinCB17 zip "$ZIP_COMMAND" "${TOZIP_MINGW17}"

makezip WinVS2015 zip "$ZIP_COMMAND" "${TOZIP_VISUAL2015}"

makezip MacOS zip "$ZIP_COMMAND" "${TOZIP_MACOS}"

makezip Linux tgz "$TGZ_COMMAND" "${TOZIP_LINUX}"

#pause

cd ${GRAPIC}
