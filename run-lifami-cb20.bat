
@echo "==================== del build ===================="
del /S /Q build

@echo "==================== premake CB ===================="
.\script\premake5.exe --os=windows --file=premake5.lua --cb-version=cb20 --lifami codeblocks 
.\script\premake5.exe --os=windows --file=premake5.lua --cb-version=cb20 --lifami gmake


@echo "==================== copy dll =>bin ===================="
mkdir bin
copy extern\mingw-cb20\bin\*.dll bin

pause
