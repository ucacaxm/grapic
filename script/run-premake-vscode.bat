
:: del /S /Q build

cd ..
.\script\premake5.exe gmake2
.\script\premake5.exe --scripts=script/vscode vscode

mkdir bin
copy extern\mingw-cb20\bin\*.dll bin
:: copy extern\visual2015\bin\*.dll bin

