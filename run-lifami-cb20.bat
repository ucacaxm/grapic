
:: @echo "==================== del build ===================="
:: del /S /Q build

@echo "==================== premake CB ===================="
.\script\premake5.exe --os=windows --file=premake5.lua --lifami codeblocks 
.\script\premake5.exe --os=windows --file=premake5.lua --lifami gmake


@echo "==================== copy dll =>bin ===================="
mkdir bin
copy extern\mingw-cb20\bin\*.dll bin

pause
