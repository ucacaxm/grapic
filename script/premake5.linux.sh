#!/bin/bash
if command -v premake5 > /dev/null 2>&1; then
    premake5 $*
else
    if [ -x premake5.linux ]; then
        premake5.linux $*
    else
        echo "Installer premake5 svp"
    fi
fi
