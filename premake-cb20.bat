
:: del /S /Q build

.\script\premake4.exe --os=windows --file=premake4.lua --cb-version=cb20 codeblocks 
.\script\premake4.exe --os=windows --file=premake4.lua --cb-version=cb20 gmake

copy extern\mingw-cb20\bin\*.dll bin

pause
