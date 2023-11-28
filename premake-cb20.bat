
:: del /S /Q build

.\script\premake5.exe --os=windows --file=premake5.lua --cb-version=cb20 codeblocks 
.\script\premake5.exe --os=windows --file=premake5.lua --cb-version=cb20 gmake

copy extern\mingw-cb20\bin\*.dll bin

pause
