
del /S /Q build

.\script\premake5.exe --os=windows --file=premake5.lua --cb-version=cb20 codeblocks 
.\script\premake5.exe --os=windows --file=premake5.lua gmake
@REM .\script\premake5.exe --os=windows --file=premake5.lua vscode

copy extern\mingw-cb20\bin\*.dll bin

pause
