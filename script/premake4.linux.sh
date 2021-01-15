#!/bin/bash

if command -v premake4 > /dev/null 2>&1; then
    premake4 $*
elif command -v script/premake4.linux > /dev/null 2>&1; then
    script/premake4.linux $*
else
    echo "Installer premake svp avec la commande : sudo apt install g++ premake4 zip doxygen libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev libsdl2-ttf-dev"
fi
