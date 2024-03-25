#!/bin/bash
if command -v premake5 > /dev/null 2>&1; then
    premake5
else
    if [ -x ./script/premake5.linux ]; then
        ./script/premake5.linux --os=linux --file=premake5.lua --lifami codeblocks 
        ./script/premake5.linux --os=linux --file=premake5.lua --lifami gmake
    else
        echo "Installer premake5 svp"
    fi
fi
