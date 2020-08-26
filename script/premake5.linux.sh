#!/bin/bash
if command -v premake5 > /dev/null 2>&1; then
    premake5 $*
else
    if [ -x script/premake5.exe.linux ]; then
        script/premake5.exe.linux $*
    else
        echo "Installer premake5 svp"
    fi
fi
