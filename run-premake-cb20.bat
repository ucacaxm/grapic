

:: @echo "==================== del build ===================="
:: del /S /Q build

.\script\premake5.exe --version

@echo "==================== premake CB ===================="
.\script\premake5.exe --os=windows --file=premake5.lua codeblocks 
@echo "==================== premake gmake ===================="
.\script\premake5.exe --os=windows --file=premake5.lua gmake

@echo "==================== copy dll =>bin ===================="
dir
mkdir bin
copy extern\mingw-cb20\bin\*.dll bin

pause
