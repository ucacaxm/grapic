
del /S /Q build

.\script\premake4.exe --os=windows gmake lifami.lua
.\script\premake4.exe --os=windows codeblocks lifami.lua

.\script\premake4.exe --os=linux gmake lifami.lua
.\script\premake4.exe --os=linux codeblocks lifami.lua

.\script\premake4.exe --os=macosx gmake lifami.lua
.\script\premake4.exe --os=macosx xcode4 lifami.lua

mkdir apps\LIFAMI

copy  apps\start\main_start.cpp apps\LIFAMI\TPComplex.cpp
copy  apps\start\main_start.cpp apps\LIFAMI\TPInterpolation.cpp
copy  apps\start\main_start.cpp apps\LIFAMI\TPParticle.cpp
copy  apps\start\main_start.cpp apps\LIFAMI\TPParticleSpring.cpp
copy  apps\start\main_start.cpp apps\LIFAMI\TPGameOfLife.cpp
copy  apps\start\main_start.cpp apps\LIFAMI\TPWolfRabbit.cpp
copy  apps\start\main_start.cpp apps\LIFAMI\TPColorInsects.cpp
copy  apps\start\main_start.cpp apps\LIFAMI\TPIceScream.cpp
echo "dir apps\LIFAMI"
dir apps\LIFAMI

pause
