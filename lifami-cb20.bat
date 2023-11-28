
del /S /Q build
.\script\premake5.exe --os=windows --file=premake5.lua --cb-version=cb20 --lifami codeblocks 
.\script\premake5.exe --os=windows --file=premake5.lua --cb-version=cb20 --lifami gmake
.\script\premake5.exe --os=windows --file=premake5.lua --cb-version=cb20 --lifami vscode
copy extern\mingw-cb20\bin\*.dll bin

pause
